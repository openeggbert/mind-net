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


#include "mindnet/plugins/repetition/triggers/RSessionBeforeCreateTrigger.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/plugins/repetition/models/RSession.hpp"
#include "mindnet/util/Utils.hpp"
#include "../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/GetRSessionSelectedItemsSQLiteQuery.hpp"


namespace mindnet::plugins::repetition::triggers
{
    using_loggers()

    RSessionBeforeCreateTrigger::RSessionBeforeCreateTrigger()
        : Trigger(
            "RSessionBeforeCreateTrigger",
            "Adds IDS to r_sessions during creation.",
            1000,
            {essential::Crudl::Create},
            api::TriggerPhase::Before,
            "r_session"
        )
    {
    }

    void RSessionBeforeCreateTrigger::run_before_or_after(
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
        models::RSession r_session;
        r_session.from_values(fields);

        auto token = api::AccessTokenContext(user_id, "", 200);
        std::vector<int> ids;

        switch (r_session.algorithm)
        {
        case enums::RepetitionAlgorithm::Repetition0: break;
        case enums::RepetitionAlgorithm::Repetition2: break;
        case enums::RepetitionAlgorithm::Repetition4: break;
        case enums::RepetitionAlgorithm::Repetition18: break;
        default:
            {
                validation_result = {400, "Unsupported algorithm."};
                return;
            }
        }
        nlohmann::json req;
        req["r_session"] = r_session.to_json();

        r_session.selected_items = call_query(db::sqlite::queries::QUERY_GetRSessionSelectedItemsQuery, req).dump();
        fields = r_session.to_values();
    }
}
