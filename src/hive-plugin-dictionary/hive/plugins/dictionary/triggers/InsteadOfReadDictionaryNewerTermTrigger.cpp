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

#include "hive/plugins/dictionary/triggers/InsteadOfReadDictionaryNewerTermTrigger.hpp"
#include "hive/essential/Global.hpp"
#include "hive/api/AccessTokenContext.hpp"
#include <string>
#include <vector>
#include "../../../../../../include/hive-db-sqlite/hive/db/sqlite/queries/dictionary/FindDictionaryOlderNewerTermsSQLiteQuery.hpp"
#include "hive/plugins/dictionary/models/DictionaryNewerTerm.hpp"
#include "hive/plugins/dictionary/models/DictionaryTerm.hpp"
#include "hive/plugins/dictionary/models/DictionaryTermMetric.hpp"

#include "hive/util/Utils.hpp"

namespace hive::plugins::dictionary::triggers
{
    using_loggers()

    InsteadOfReadDictionaryNewerTermTrigger::InsteadOfReadDictionaryNewerTermTrigger()
        : Trigger(
            "InsteadOfReadDictionaryNewerTermTrigger",
            "Calls custom sql for read older term request",
            1000,
            {essential::Crudl::Read},
            api::TriggerPhase::InsteadOf,
            "dictionary_newer_term"
        )
    {
    }

    std::optional<std::pair<entity_fields, api::OperationResult>> InsteadOfReadDictionaryNewerTermTrigger::
    run_instead_of_read(int stack_depth, api::OperationResult& validation_result, const model::ModelDefinition& def,
                        identification user_id, identification id)
    {
        api::AccessTokenContext ctx{true};
        auto read_term = run_read(models::DICTIONARY_TERM_DEFINITION, ctx, id, stack_depth);

        models::DictionaryNewerTerm result;
        if (!read_term.second)
        {
            auto values = result.to_values();
            auto r = std::make_pair<entity_fields, api::OperationResult>(std::move(values), {
                                                                             500, "Internal server error."
                                                                         });
            return r;
        }
        nlohmann::json req;

        models::DictionaryTerm term;
        term.from_values(read_term.first);

        auto dictionary_map_id = term.dictionary_map_id;
        req["dictionary_map_id"] = dictionary_map_id;
        auto dictionary_term_id = id;
        req["dictionary_term_id"] = dictionary_term_id;
        req["mode"] = "newer";

        nlohmann::json res;

        try
        {
            res = call_query(db::sqlite::queries::dictionary::QUERY_FindDictionaryOlderNewerTerms, req);

            if (res.contains("error"))
            {
                auto values = result.to_values();
                auto r = std::make_pair<entity_fields, api::OperationResult>(
                    std::move(values), {500, "Internal server error."});
                return r;
            }

            info << res.dump() << commit;
            info << "Query FindDictionaryNewerTerms successful" << commit;
        }
        catch (std::exception& e)
        {
            err << "Query FindDictionaryNewerTerms failed " << e.what() << commit;
            auto values = result.to_values();
            auto r = std::make_pair<entity_fields, api::OperationResult>(std::move(values), {
                                                                             500, "Internal server error."
                                                                         });
            return r;
        }

        result.set_id(id);
        result.dictionary_map_id = dictionary_map_id;
        result.dictionary_term_id = id;
        result.newer_term_id = res["id"];
        auto values = result.to_values();
        int64_t now = static_cast<int64_t>(util::Utils::current_unix_timestamp_ms());
        values[1] = now;
        values[2] = now;

        auto v = result.to_values();
        auto r =
            std::make_pair<entity_fields, api::OperationResult>
            (std::move(v), ok_result);

        return r;
    }
}
