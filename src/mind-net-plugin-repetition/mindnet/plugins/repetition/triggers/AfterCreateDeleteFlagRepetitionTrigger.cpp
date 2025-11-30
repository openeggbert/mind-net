/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mindnet/plugins/repetition/triggers/AfterCreateDeleteFlagRepetitionTrigger.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/api/IPersistence.hpp"
#include "mindnet/plugins/repetition/models/R0State.hpp"
#include "mindnet/plugins/repetition/models/R18State.hpp"
#include "mindnet/plugins/repetition/models/R2State.hpp"
#include "mindnet/plugins/repetition/models/R4State.hpp"
#include "mindnet/plugins/repetition/validators/R0StateValidator.hpp"
#include "mindnet/plugins/slipbox/models/Content.hpp"
#include "mindnet/plugins/slipbox/models/Flag.hpp"
#include "mindnet/plugins/slipbox/models/Note.hpp"
#include "mindnet/util/Utils.hpp"

namespace mindnet::plugins::repetition::triggers
{
    using_loggers()

    AfterCreateDeleteFlagRepetitionTrigger::AfterCreateDeleteFlagRepetitionTrigger()
        : Trigger(
            "AfterCreateDeleteFlagRepetitionTrigger",
            "Updates repetition state tables, if the tag is repetition",
            0,
            {essential::Crudl::Create, essential::Crudl::Delete},
            api::TriggerPhase::After,
            "flag"
        )
    {
    }

    void AfterCreateDeleteFlagRepetitionTrigger::run_before_or_after(
        mindnet::essential::Crudl operation,
        int stack_depth,
        api::OperationResult& validation_result,
        api::OperationResult& action_result,
        const mindnet::model::ModelDefinition def,
        identification user_id,
        identification id,
        entity_fields& fields,
        entity_fields& old_fields,
        const orm::QueryParams query_params)
    {
        if (validation_result.ko()) return;
        if (action_result.ko()) return;

        auto user_token = api::AccessTokenContext(user_id, "", 200);

        slipbox::models::Flag flag;
        // if (operation == essential::Crudl::Create)
        flag.from_values(fields);
        // if (operation == essential::Crudl::Delete)
        // {
        //
        //     auto read_flag =  run_read(def, user_token, id, stack_depth);
        //     if (!read_flag.second)
        //     {
        //         err << "Reading flag failed: " << read_flag.second.error << commit;
        //         return;
        //     }
        //         flag.from_values(read_flag.first);
        // }


        if (flag.title != "repetition") return;

        bool new_eligible = operation == essential::Crudl::Create;

        auto system_token = api::AccessTokenContext(true);

        identification note_id = flag.note_id;

        struct StateDef
        {
            int version;
            const mindnet::model::ModelDefinition* def;
        };

        std::vector<StateDef> states = {
            {0, &models::R0_STATE_DEFINITION},
            {2, &models::R2_STATE_DEFINITION},
            {4, &models::R4_STATE_DEFINITION},
            {18, &models::R18_STATE_DEFINITION}
        };

        orm::QueryParams state_params;
        state_params.add_filter("note_id", note_id);

        int64_t now = util::Utils::current_unix_timestamp_ms();

        for (auto& sd : states)
        {
            auto list_res = run_list(*sd.def, user_token, state_params, stack_depth);
            if (list_res.second.ko())
            {
                err << "Loading states failed: " << list_res.second.error << commit;
                continue;
            }
            if (list_res.first.empty())
            {
                warn << "No states found for note_id=" << note_id << " (r_version=" << sd.version << ")" << commit;
                continue;
            }
            for (auto& state : list_res.first)
            {
                entity_fields v = state; // copy
                identification state_id = get<i64>(v[0]);
                sd.def->update_entity_field(v, columns::R0StateColumns::ELIGIBLE, new_eligible);
                sd.def->update_entity_field(v, columns::R0StateColumns::UPDATED_AT, now);
                debug << "Updating table " << sd.def->get_model_name()
                    << " id=" << state_id
                    << " eligible=" << new_eligible << commit;

                auto updated = run_update(*sd.def, system_token, state_id, v, stack_depth);
                if (updated) run_invalidate(*sd.def, state_id);
            }
        }
    }
}
