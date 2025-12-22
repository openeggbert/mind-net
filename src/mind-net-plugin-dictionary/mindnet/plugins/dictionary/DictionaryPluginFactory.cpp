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
#include "mindnet/plugins/dictionary/DictionaryPluginFactory.hpp"

#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/dictionary/FindNextDictionaryNotePositionSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/dictionary/FindDictionaryTermsSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/dictionary/FindDictionaryTagTypesSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/dictionary/FindDictionaryIndexTypesSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/dictionary/FindDictionarySourceTypesSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/dictionary/FindDictionaryTermMetricsSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/dictionary/FindDictionaryFlagsSQLiteQuery.hpp"
#include "mindnet/api/Plugin.hpp"
#include "mindnet/api/PluginFactory.hpp"
#include "mindnet/db/sqlite/queries/dictionary/FindDictionarySearchesSQLiteQuery.hpp"
#include "mindnet/db/sqlite/queries/dictionary/FindDictionaryTermAliasesSQLiteQuery.hpp"
#include "mindnet/db/sqlite/queries/dictionary/FindDictionaryTermsForReviewSQLiteQuery.hpp"
#include "mindnet/db/sqlite/queries/dictionary/FindDictionaryTermsViaAdvancedSearchSQLiteQuery.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryFlagValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryLinkValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryMapValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryNoteValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryReviewValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionarySourceTypeValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionarySourceValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryState18Validator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTagValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTagTypeValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTagTypeFulltextValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionarySourceTypeFulltextValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTermAliasValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTermValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTermFulltextValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTermSearchValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTermVisitValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryPinnedTermValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTermForReviewValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTermMetricValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionarySearchValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryIndexTypeValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryIndexTypeFulltextValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryIndexValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryFlagFulltextValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTermAliasFulltextValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionarySearchFulltextValidator.hpp"

#include "mindnet/plugins/dictionary/migrations/DictionarySQLiteMigrationScripts.hpp"
#include "mindnet/plugins/dictionary/triggers/BeforeCreateDictionaryNoteTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/InsteadOfListDictionaryTermFulltextTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/InsteadOfListDictionaryTagTypeFulltextTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/InsteadOfListDictionaryIndexTypeFulltextTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/InsteadOfListDictionarySourceTypeFulltextTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/InsteadOfListDictionaryTermMetricsTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/InsteadOfListDictionaryFlagsFulltextTrigger.hpp"
#include "mindnet/plugins/dictionary/jobs/DictionaryHtmlExportJob.hpp"
#include "mindnet/plugins/dictionary/triggers/DictionaryReviewAfterCreateTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/InsteadOfListDictionarySearchesFulltextTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/InsteadOfListDictionaryTermAliasesFulltextTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/InsteadOfListDictionaryTermSearchTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/InsteadOfListDictionaryTermsForReviewTrigger.hpp"

namespace mindnet::plugins::dictionary
{
    api::PluginPtr DictionaryPluginFactory::create(std::shared_ptr<api::RepositoryFactory>& repository_factory) const
    {
        auto plugin = std::make_shared<api::Plugin>(
            DICTIONARY_PLUGIN_NAME,
            "dictionary",
            std::vector<std::string>{
                "dictionary"
            }
        );

        REGISTER_MIGRATIONS(Dictionary, SQLite)

        REGISTER_MODEL(dictionary_map, DictionaryMap, DICTIONARY_MAP)
        REGISTER_MODEL(dictionary_term, DictionaryTerm, DICTIONARY_TERM)
        REGISTER_MODEL(dictionary_link, DictionaryLink, DICTIONARY_LINK)
        REGISTER_MODEL(dictionary_flag, DictionaryFlag, DICTIONARY_FLAG)
        REGISTER_MODEL(dictionary_tag, DictionaryTag, DICTIONARY_TAG)
        REGISTER_MODEL(dictionary_note, DictionaryNote, DICTIONARY_NOTE)
        REGISTER_MODEL(dictionary_index, DictionaryIndex, DICTIONARY_INDEX)
        REGISTER_MODEL(dictionary_source, DictionarySource, DICTIONARY_SOURCE)

        REGISTER_MODEL(dictionary_pinned_term, DictionaryPinnedTerm, DICTIONARY_PINNED_TERM)
        REGISTER_MODEL(dictionary_term_alias, DictionaryTermAlias, DICTIONARY_TERM_ALIAS)

        REGISTER_MODEL(dictionary_tag_type, DictionaryTagType, DICTIONARY_TAG_TYPE)
        REGISTER_MODEL(dictionary_index_type, DictionaryIndexType, DICTIONARY_INDEX_TYPE)
        REGISTER_MODEL(dictionary_source_type, DictionarySourceType, DICTIONARY_SOURCE_TYPE)
        REGISTER_MODEL(dictionary_search, DictionarySearch, DICTIONARY_SEARCH)

        REGISTER_MODEL(dictionary_term_visit, DictionaryTermVisit, DICTIONARY_TERM_VISIT)
        REGISTER_MODEL(dictionary_term_metric, DictionaryTermMetric, DICTIONARY_TERM_METRIC)
        REGISTER_MODEL(dictionary_review, DictionaryReview, DICTIONARY_REVIEW)
        REGISTER_MODEL(dictionary_state_18, DictionaryState18, DICTIONARY_STATE_18)

        REGISTER_MODEL(dictionary_term_fulltext, DictionaryTermFulltext, DICTIONARY_TERM_FULLTEXT)
        REGISTER_MODEL(dictionary_term_for_review, DictionaryTermForReview, DICTIONARY_TERM_FOR_REVIEW)
        REGISTER_MODEL(dictionary_term_search, DictionaryTermSearch, DICTIONARY_TERM_SEARCH)
        REGISTER_MODEL(dictionary_tag_type_fulltext, DictionaryTagTypeFulltext, DICTIONARY_TAG_TYPE_FULLTEXT)
        REGISTER_MODEL(dictionary_index_type_fulltext, DictionaryIndexTypeFulltext, DICTIONARY_INDEX_TYPE_FULLTEXT)
        REGISTER_MODEL(dictionary_source_type_fulltext, DictionarySourceTypeFulltext, DICTIONARY_SOURCE_TYPE_FULLTEXT)
        REGISTER_MODEL(dictionary_flag_fulltext, DictionaryFlagFulltext, DICTIONARY_FLAG_FULLTEXT)
        REGISTER_MODEL(dictionary_term_alias_fulltext, DictionaryTermAliasFulltext, DICTIONARY_TERM_ALIAS_FULLTEXT)
        REGISTER_MODEL(dictionary_search_fulltext, DictionarySearchFulltext, DICTIONARY_SEARCH_FULLTEXT)

        plugin->register_trigger(std::make_shared<triggers::BeforeCreateDictionaryNoteTrigger>());
        plugin->register_trigger(std::make_shared<triggers::InsteadOfListDictionaryTermFulltextTrigger>());
        plugin->register_trigger(std::make_shared<triggers::InsteadOfListDictionaryTagTypeFulltextTrigger>());
        plugin->register_trigger(std::make_shared<triggers::InsteadOfListDictionarySourceTypeFulltextTrigger>());
        plugin->register_trigger(std::make_shared<triggers::InsteadOfListDictionaryIndexTypeFulltextTrigger>());
        plugin->register_trigger(std::make_shared<triggers::InsteadOfListDictionaryTermMetricsTrigger>());
        plugin->register_trigger(std::make_shared<triggers::InsteadOfListDictionaryFlagsFulltextTrigger>());
        plugin->register_trigger(std::make_shared<triggers::InsteadOfListDictionaryTermAliasesFulltextTrigger>());
        plugin->register_trigger(std::make_shared<triggers::InsteadOfListDictionarySearchesFulltextTrigger>());
        plugin->register_trigger(std::make_shared<triggers::InsteadOfListDictionaryTermSearchTrigger>());
        plugin->register_trigger(std::make_shared<triggers::InsteadOfListDictionaryTermsForReviewTrigger>());
        plugin->register_trigger(std::make_shared<triggers::DictionaryReviewAfterCreateTrigger>());

        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::dictionary::FindNextDictionaryNotePositionSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::dictionary::FindDictionaryTermsSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::dictionary::FindDictionaryTermsViaAdvancedSearchSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::dictionary::FindDictionaryTagTypesSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::dictionary::FindDictionarySourceTypesSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::dictionary::FindDictionaryIndexTypesSQLiteQuery>());;
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::dictionary::FindDictionaryTermMetricsSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::dictionary::FindDictionaryFlagsSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::dictionary::FindDictionaryTermAliasesSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::dictionary::FindDictionarySearchesSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::dictionary::FindDictionaryTermsForReviewSQLiteQuery>());

        plugin->register_job(std::make_shared<mindnet::plugins::dictionary::jobs::DictionaryHtmlExportJob>());

        plugin->register_library_file("markdown-it.min.js");
        plugin->register_library_file("highlight.min.js");
        plugin->register_library_file("markdown-it-emoji.min.js");
        plugin->register_library_file("github.min.css");

        plugin->register_library_file("d_markdown.js");
        plugin->register_library_file("d_entities.js");
        plugin->register_library_file("d_enums.js");
        plugin->register_library_file("d_globals.js");
        plugin->register_library_file("d_window.js");
        plugin->register_library_file("d_search.js");
        plugin->register_library_file("d_dom.js");
        plugin->register_library_file("d_styles.js");
        plugin->register_library_file("d_styles_enums.js");

        plugin->close_for_changes();
        return plugin;
    }
}
