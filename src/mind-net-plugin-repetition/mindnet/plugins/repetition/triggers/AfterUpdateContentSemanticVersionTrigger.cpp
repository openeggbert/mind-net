//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/plugins/repetition/triggers/AfterUpdateContentSemanticVersionTrigger.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/api/IPersistence.hpp"
#include "mindnet/plugins/repetition/models/R0State.hpp"
#include "mindnet/plugins/repetition/models/R18State.hpp"
#include "mindnet/plugins/repetition/models/R2State.hpp"
#include "mindnet/plugins/repetition/models/R4State.hpp"
#include "mindnet/plugins/repetition/validators/R0StateValidator.hpp"
#include "mindnet/plugins/slipbox/models/Content.hpp"
#include "mindnet/plugins/slipbox/models/Note.hpp"
#include "mindnet/util/Utils.hpp"

namespace mindnet::plugins::repetition::triggers
{
    using_loggers()

    AfterUpdateContentSemanticVersionTrigger::AfterUpdateContentSemanticVersionTrigger()
        : Trigger(
            "AfterUpdateContentSemanticVersionTrigger",
            "Updates state tables, if needed",
            0,
            {essential::Crudl::Update},
            api::TriggerPhase::After,
            "content"
        )
    {
    }

    // Adjusts stability based on content change ratio.
    // change_ratio: value between 0 and 100 (e.g., 25 means 25% change)
    // S_old: previous stability value
    // S_min: minimum allowed stability (e.g., 1.0)

    double adjust_stability(double change_ratio, double S_old, double S_min = 1.0) {
        // Clamp change_ratio to [0, 100]
        if (change_ratio < 0) change_ratio = 0;
        if (change_ratio > 100) change_ratio = 100;

        // Compute new stability
        double S_new = S_old * (100.0 - change_ratio) / 100.0;

        // Enforce minimum stability
        if (S_new < S_min) S_new = S_min;

        return S_new;
    }

    void AfterUpdateContentSemanticVersionTrigger::run_before_or_after(
        mindnet::essential::Crudl operation,
        int stack_depth,
        api::OperationResult& validation_result,
        api::OperationResult& action_result,
        const mindnet::model::ModelDefinition def,
        i64 user_id,
        i64 id,
        entity_fields& fields,
        entity_fields& old_fields,
        const orm::QueryParams query_params)
    {
        mindnet::plugins::slipbox::models::Content old_content;
        mindnet::plugins::slipbox::models::Content new_content;
        old_content.from_values(old_fields);
        new_content.from_values(fields);
        auto& old_semantic_version = old_content.semantic_version;
        auto& new_semantic_version = new_content.semantic_version;
        auto& change_ratio = new_content.change_ratio;

        bool change_20_50 = change_ratio >= 20 && change_ratio < 50;
        bool change_50_100 = change_ratio >= 50;

        if (old_semantic_version == new_semantic_version)
        {
            //nothing to do
            return;
        }

        if (change_ratio < 20)
        {
            debug << "Content changed only " << change_ratio << "% → no state updates." << commit;
            return;
        }

        auto token = api::AccessTokenContext(user_id, "", 200);

        orm::QueryParams note_params;
        note_params.add_filter("content_id", new_content.get_id());
        auto note_result = run_list(slipbox::models::NOTE_DEFINITION, token, note_params, stack_depth);
        if (note_result.second.ko())
        {
            err << "Loading note failed: " << note_result.second.error << commit;
            return;
        }
        if (note_result.first.empty())
        {
            err << "Note not found for content with ID: " << new_content.get_id() << commit;
            return;
        }
        slipbox::models::Note note;
        note.from_values(note_result.first.at(0));

        models::R0State r0_state;
        models::R2State r2_state;
        models::R4State r4_state;
        models::R18State r18_state;

        constexpr static int64_t MILLISECONDS_PER_DAY = 86400L * 1000;
        constexpr int64_t ONE_HOUR_MS = 3600LL * 1000;

        for (int r_version : {0, 2, 4, 18})
        {
            orm::QueryParams state_params;
            state_params.add_filter("note_id", note.get_id());
            const model::ModelDefinition* model_definition = &models::R0_STATE_DEFINITION;
            switch (r_version)
            {
            case 0: model_definition = &models::R0_STATE_DEFINITION;break;
            case 2: model_definition = &models::R2_STATE_DEFINITION;break;
            case 4: model_definition = &models::R4_STATE_DEFINITION;break;
            case 18: model_definition = &models::R18_STATE_DEFINITION;break;
            default: err << "Unsupported algorithm " << r_version << commit; continue;
            }
            auto state_result = run_list(*model_definition, token, state_params, stack_depth);
            if (state_result.second.ko())
            {
                err << "Loading states failed: " << state_result.second.error << commit;
                continue;
            }
            if (state_result.first.empty())
            {
                warn << "No states found for note_id=" << note.get_id() << " (r_version=" << r_version << ")" << commit;
                continue;
            }
            for (auto& state : state_result.first)
            {
                switch (r_version)
                {
                case 0: r0_state.from_values(state); break;
                case 2: r2_state.from_values(state); break;
                case 4: r4_state.from_values(state); break;
                case 18: r18_state.from_values(state); break;
                default: err << "Unsupported algorithm " << r_version << commit; continue;
                }
                int64_t now = util::Utils::current_unix_timestamp_ms();
                if (r_version == 0)
                {
                    if (change_20_50)
                    {
                        const static std::vector<int> SM0 = {1, 6, 16, 35, 62, 100, 150, 210, 300, 420};

                        auto it = std::lower_bound(SM0.begin(), SM0.end(), r0_state.interval);
                        if (it != SM0.begin()) {
                            // move back 1 step
                            --it;
                            r0_state.interval = std::max(1, *it);
                            r0_state.next_review = r0_state.last_review + (int64_t)r0_state.interval * MILLISECONDS_PER_DAY;
                        }
                    } else
                    {
                        r0_state.repetitions = 0;
                        r0_state.interval = 1;
                        r0_state.next_review = r0_state.last_review + r0_state.interval * MILLISECONDS_PER_DAY;
                        r0_state.last_quality = 0;
                    }
                    if (r0_state.next_review < now) r0_state.next_review = now + ONE_HOUR_MS;

                    r0_state.last_seen_semantic_version = new_semantic_version;
                    r0_state.content_modified_since_last_review = true;
                    auto v = r0_state.to_values();

                    debug << "Updating table " << model_definition->get_model_name()
      << " id=" << r0_state.get_id()
      << " version=" << r_version
      << " change_ratio=" << change_ratio << commit;

                        run_update(*model_definition, token, r0_state.get_id(), v, stack_depth);
                }
                if (r_version == 2)
                {
                    if (change_20_50) {
                        r2_state.interval = std::max(1.0, r2_state.interval * 0.5);
                        r2_state.next_review = r2_state.last_review + (int64_t)std::llround(r2_state.interval * MILLISECONDS_PER_DAY);
                    } else {
                        r2_state.repetitions = 0;
                        r2_state.interval = 1;
                        r2_state.ef_times_100 = std::max(100, r2_state.ef_times_100 - 20);
                        r2_state.last_quality = 0;
                        r2_state.next_review = r2_state.last_review + MILLISECONDS_PER_DAY;
                    }
                    if (r2_state.next_review < now) r2_state.next_review = now + ONE_HOUR_MS;

                        r2_state.last_seen_semantic_version = new_semantic_version;
                        r2_state.content_modified_since_last_review = true;
                        auto v = r2_state.to_values();
                    debug << "Updating table " << model_definition->get_model_name()
<< " id=" << r2_state.get_id()
<< " version=" << r_version
<< " change_ratio=" << change_ratio << commit;
                        run_update(*model_definition, token, r2_state.get_id(), v, stack_depth);

                }
                if (r_version == 4)
                {

                    if (change_20_50) {
                        r4_state.interval = std::max(1.0, r4_state.interval * 0.5);
                        r4_state.next_review = r4_state.last_review + (int64_t)std::llround(r4_state.interval * MILLISECONDS_PER_DAY);
                    } else {
                        r4_state.repetitions = 0;
                        r4_state.interval = 1;
                        r4_state.ef_times_100 = std::max(100, r4_state.ef_times_100 - 20);
                        r4_state.last_quality = 0;
                        r4_state.next_review = r4_state.last_review + MILLISECONDS_PER_DAY;
                    }
                    if (r4_state.next_review < now) r4_state.next_review = now + ONE_HOUR_MS;

                    r4_state.last_seen_semantic_version = new_semantic_version;
                    r4_state.content_modified_since_last_review = true;
                    auto v = r4_state.to_values();
                    debug << "Updating table " << model_definition->get_model_name()
<< " id=" << r4_state.get_id()
<< " version=" << r_version
<< " change_ratio=" << change_ratio << commit;
                    run_update(*model_definition, token, r4_state.get_id(), v, stack_depth);

                }
                if (r_version == 18)
                {
                    if (change_20_50 && r18_state.last_interval_times_100 != 0) {
                        double S_old = r18_state.stability_times_100 / 100.0;
                        double S_new = adjust_stability(change_ratio, S_old, 8.0);
                        r18_state.stability_times_100 = (int)std::round(S_new * 100.0);

                        int64_t last_interval_ms = now - r18_state.last_review;
                        if (last_interval_ms < 0) last_interval_ms = 0;

                        double elapsed_days = (double)last_interval_ms / MILLISECONDS_PER_DAY;
                        double new_interval_days = std::max(1.0, elapsed_days * 0.5);

                        r18_state.last_interval_times_100 = (int)std::round(elapsed_days * 100.0);
                        r18_state.next_review = now + (int64_t)std::llround(new_interval_days * MILLISECONDS_PER_DAY);
                    } else {
                        r18_state.stability_times_100 = 800;
                        r18_state.last_interval_times_100 = 0;
                        r18_state.repetitions = 0;
                        r18_state.lapses = 0;
                        r18_state.last_quality = 0;
                        r18_state.next_review = now + ONE_HOUR_MS;
                        r18_state.last_review = now;
                    }

                    r18_state.last_seen_semantic_version = new_semantic_version;
                    r18_state.content_modified_since_last_review = true;
                    auto v = r18_state.to_values();
                    debug << "Updating table " << model_definition->get_model_name()
<< " id=" << r18_state.get_id()
<< " version=" << r_version
<< " change_ratio=" << change_ratio << commit;
                    run_update(*model_definition, token, r18_state.get_id(), v, stack_depth);

                }
            }
        }
        info << "Semantic version updated → spaced repetition adjusted (ratio="
     << change_ratio << "%, sem_v=" << new_semantic_version << ")" << commit;
    }
}
