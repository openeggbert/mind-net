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

#include "mindnet/plugins/dictionary/triggers/InsteadOfListDictionaryTermsForReviewTrigger.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include <string>
#include <vector>
#include "../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/dictionary/FindDictionaryTermsForReviewSQLiteQuery.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryTerm.hpp"

#include "mindnet/util/Utils.hpp"

namespace mindnet::plugins::dictionary::triggers
{
    using_loggers()

    InsteadOfListDictionaryTermsForReviewTrigger::InsteadOfListDictionaryTermsForReviewTrigger()
        : Trigger(
            "InsteadOfListDictionaryTermsForReviewTrigger",
            "Calls custom sql for list term for review request",
            1000,
            {essential::Crudl::List},
            api::TriggerPhase::InsteadOf,
            "dictionary_term_for_review"
        )
    {
    }

    std::optional<std::pair<std::vector<entity_fields>, api::OperationResult>>  InsteadOfListDictionaryTermsForReviewTrigger::
    run_instead_of_list(
            int stack_depth,
            api::OperationResult& validation_result,
            const model::ModelDefinition& def,
            identification user_id,
            const orm::QueryParams& query_params)
    {
        std::vector<entity_fields> v;
        std::optional<std::pair<std::vector<entity_fields>, api::OperationResult>> result;

        nlohmann::json req;
        auto& f = query_params.filters;

        identification dictionary_map_id = stoll(f.at("dictionary_map_id"));
        identification dictionary_search_id = f.contains("dictionary_search_id") ? stoll(f.at("dictionary_search_id")) : 0;
        bool search = dictionary_search_id != 0;

        bool is_due = f.contains("is_due") ? stoll(f.at("is_due")) : false;
        bool is_new = f.contains("is_new") ? stoll(f.at("is_new")) : false;
        bool is_not_due = f.contains("is_not_due") ? stoll(f.at("is_not_due")) : false;
        bool include_empty_definition = f.contains("include_empty_definition") ? stoll(f.at("include_empty_definition")) : false;

        req["dictionary_map_id"] = dictionary_map_id;
        if (f.contains("user_id") && stoll(f.at("user_id")) != user_id)
        {
            err << "Query FindDictionaryTermsForReview failed: user_id must be yours" << commit;
            std::vector<entity_fields> v0;
            result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(
                std::move(v0), {400, "Bad request. user_id must be yours"});
            return result;
        }
        req["user_id"] = user_id;
        int algorithm = f.contains("algorithm") ? stoll(f.at("algorithm")) : 4;
        if (algorithm != 4)
        {
            err << "Query FindDictionaryTermsForReview failed: algorithm != 4" << commit;
            std::vector<entity_fields> v0;
            result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(
                std::move(v0), {400, "Bad request. algorithm != 4"});
            return result;
        }
        req["algorithm"] = algorithm;
        req["is_due"] = is_due;
        req["is_new"] = is_new;
        req["is_not_due"] = is_not_due;
        req["include_empty_definition"] = include_empty_definition;

        // int page_size = query_params.page_size;
        // int page_number = query_params.page_number;
        // req["page_size"] = page_size;
        // req["page_number"] = page_number;

        std::vector<identification> term_ids;
        api::AccessTokenContext ctx{true};

        if (!search)
        try
        {
            nlohmann::json res = call_query(db::sqlite::queries::dictionary::QUERY_FindDictionaryTermsForReview, req);

            if (res.contains("error"))
            {
                std::vector<entity_fields> v0;
                result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v0), {500, "Internal server error."});
                return result;
            }

            term_ids = res["term_ids"].get<std::vector<identification>>();
            info << res.dump() << commit;
            info << "Query FindDictionaryTermsForReview successful" << commit;

            for (auto& term_id:term_ids)
            {
                const auto& read_term = run_read(models::DICTIONARY_TERM_DEFINITION, ctx, term_id, 0);
                if (!read_term.second)
                {
                    err << "Query FindDictionaryTermsForReview failed " << read_term.second.error << commit;
                    std::vector<entity_fields> v0;
                    result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v0), {500, "Internal server error."});
                    return result;
                }
                models::DictionaryTerm term;
                term.from_values(read_term.first);

                models::DictionaryTermForReview term_for_review;
                term_for_review.set_id(term_id);
                term_for_review.dictionary_term_id = term_id;
                term_for_review.dictionary_map_id = dictionary_map_id;
                term_for_review.user_id = user_id;
                term_for_review.dictionary_search_id = dictionary_search_id;

                term_for_review.title = term.title;
                term_for_review.disambiguation = term.disambiguation;
                term_for_review.definition = term.definition;
                term_for_review.algorithm = algorithm;
                term_for_review.is_due = is_due;
                term_for_review.is_new = is_not_due;
                term_for_review.is_not_due = is_not_due;
                term_for_review.include_empty_definition = include_empty_definition;

                auto values = term_for_review.to_values();
                int64_t now = static_cast<int64_t>(util::Utils::current_unix_timestamp_ms());
                values[1] = now;
                values[2] = now;
                v.push_back(values);
            }
        }
        catch (std::exception& e)
        {
            err << "Query FindDictionaryTermsForReview failed " << e.what() << commit;
            std::vector<entity_fields> v0;
            result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v0), {500, "Internal server error."});
            return result;
        }
        if (search)
        {

        }

        result = std::make_pair<std::vector<entity_fields>, api::OperationResult>(std::move(v), ok_result);
        return result;
    }
}