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

#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindDictionaryNextPositionSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindDictionaryNotesInMapSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindDictionaryTermsSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindDictionaryTagTypesSQLiteQuery.hpp"
#include "mindnet/api/Plugin.hpp"
#include "mindnet/api/PluginFactory.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryLinkValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryMapValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryNoteValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTagValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTagTypeValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTagTypeFulltextValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTermValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTermFulltextValidator.hpp"
#include "mindnet/plugins/dictionary/validators/DictionaryTermVisitValidator.hpp"
#include "mindnet/plugins/dictionary/migrations/DictionarySQLiteMigrationScripts.hpp"
#include "mindnet/plugins/dictionary/triggers/BeforeCreateDictionaryNoteTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/InsteadOfListDictioniaryTermFulltextTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/InsteadOfListDictionaryTagTypeFulltextTrigger.hpp"
#include "mindnet/plugins/dictionary/jobs/DictionaryHtmlExportJob.hpp"

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

        REGISTER_MODEL(dictionary_link, DictionaryLink, DICTIONARY_LINK)
        REGISTER_MODEL(dictionary_map, DictionaryMap, DICTIONARY_MAP)
        REGISTER_MODEL(dictionary_note, DictionaryNote, DICTIONARY_NOTE)
        REGISTER_MODEL(dictionary_tag, DictionaryTag, DICTIONARY_TAG)
        REGISTER_MODEL(dictionary_tag_type, DictionaryTagType, DICTIONARY_TAG_TYPE)
        REGISTER_MODEL(dictionary_tag_type_fulltext, DictionaryTagTypeFulltext, DICTIONARY_TAG_TYPE_FULLTEXT)
        REGISTER_MODEL(dictionary_term, DictionaryTerm, DICTIONARY_TERM)
        REGISTER_MODEL(dictionary_term_fulltext, DictionaryTermFulltext, DICTIONARY_TERM_FULLTEXT)
        REGISTER_MODEL(dictionary_term_visit, DictionaryTermVisit, DICTIONARY_TERM_VISIT)

        plugin->register_trigger(std::make_shared<triggers::BeforeCreateDictionaryNoteTrigger>());
        plugin->register_trigger(std::make_shared<triggers::InsteadOfListDictioniaryTermFulltextTrigger>());
        plugin->register_trigger(std::make_shared<triggers::InsteadOfListDictionaryTagTypeFulltextTrigger>());

        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::UpdateNotePathAndDepthSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::FindNotesInMapSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::FindNextSiblingOrderSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::FindPreviousAndNextNoteSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::GetQuestionIdsSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::FindTermsSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::FindTagTypesSQLiteQuery>());

        plugin->register_job(std::make_shared<mindnet::plugins::dictionary::jobs::DictionaryHtmlExportJob>());
        plugin->register_library_file("markdown-it.min.js");
        plugin->register_library_file("highlight.min.js");
        plugin->register_library_file("markdown-it-emoji.min.js");
        plugin->register_library_file("github.min.css");

        plugin->close_for_changes();
        return plugin;
    }
}