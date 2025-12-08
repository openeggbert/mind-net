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
#include "mindnet/plugins/dictionary/SlipBoxPluginFactory.hpp"

#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindNextSiblingOrderSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindNotesInMapSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindPreviousAndNextNoteSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/UpdateNotePathAndDepthSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/GetQuestionIdsSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindTermsSQLiteQuery.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindTagTypesSQLiteQuery.hpp"
#include "mindnet/api/Plugin.hpp"
#include "mindnet/api/PluginFactory.hpp"
#include "mindnet/db/sqlite/queries/FindTermsSQLiteQuery.hpp"
#include "mindnet/plugins/dictionary/validators/CollectionValidator.hpp"
#include "mindnet/plugins/dictionary/validators/CollectionItemValidator.hpp"
#include "mindnet/plugins/dictionary/validators/ContentValidator.hpp"
#include "mindnet/plugins/dictionary/validators/UrlValidator.hpp"
#include "mindnet/plugins/dictionary/validators/MapValidator.hpp"
#include "mindnet/plugins/dictionary/validators/MapCollectionValidator.hpp"
#include "mindnet/plugins/dictionary/validators/MapCollectionItemValidator.hpp"
#include "mindnet/plugins/dictionary/validators/NoteValidator.hpp"
#include "mindnet/plugins/dictionary/validators/NoteNavigationValidator.hpp"
#include "mindnet/plugins/dictionary/validators/PropertyValidator.hpp"
#include "mindnet/plugins/dictionary/validators/QuestionValidator.hpp"
#include "mindnet/plugins/dictionary/validators/LinkValidator.hpp"
#include "mindnet/plugins/dictionary/validators/TagValidator.hpp"
#include "mindnet/plugins/dictionary/validators/TagTypeValidator.hpp"
#include "mindnet/plugins/dictionary/validators/TermValidator.hpp"
#include "mindnet/plugins/dictionary/validators/SourceValidator.hpp"
#include "mindnet/plugins/dictionary/validators/IdeaValidator.hpp"
#include "mindnet/plugins/dictionary/validators/WantedNoteValidator.hpp"
#include "mindnet/plugins/dictionary/validators/AlertValidator.hpp"
#include "mindnet/plugins/dictionary/validators/FlagValidator.hpp"
#include "mindnet/plugins/dictionary/validators/ProjectValidator.hpp"
#include "mindnet/plugins/dictionary/validators/TaskValidator.hpp"
#include "mindnet/plugins/dictionary/validators/PinnedNoteValidator.hpp"
#include "mindnet/plugins/dictionary/validators/TestValidator.hpp"
#include "mindnet/plugins/dictionary/validators/TestAttemptValidator.hpp"
#include "mindnet/plugins/dictionary/validators/TestAttemptAnswerValidator.hpp"
#include "mindnet/plugins/dictionary/validators/AnnotationValidator.hpp"
#include "mindnet/plugins/dictionary/validators/TermFulltextValidator.hpp"
#include "mindnet/plugins/dictionary/validators/TagTypeFulltextValidator.hpp"
#include "mindnet/plugins/dictionary/migrations/SlipBoxSQLiteMigrationScripts.hpp"
#include "mindnet/plugins/dictionary/triggers/AfterCreateUpdateNoteTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/AfterUpdateContentTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/BeforeCreateNoteTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/BeforeUpdateContentTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/UpdateNotePathAndDepthAfterTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/InsteadOfReadNoteNavigationTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/AfterCreateTestAttemptTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/AfterCreateTestAttemptAnswerTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/InsteadOfListTermFulltextTrigger.hpp"
#include "mindnet/plugins/dictionary/triggers/InsteadOfListTagTypeFulltextTrigger.hpp"
#include "mindnet/plugins/dictionary/jobs/HtmlExportJob.hpp"

namespace mindnet::plugins::dictionary
{
    api::PluginPtr SlipBoxPluginFactory::create(std::shared_ptr<api::RepositoryFactory>& repository_factory) const
    {
        auto plugin = std::make_shared<api::Plugin>(
            SLIP_BOX_PLUGIN_NAME,
            "slip_box",
            std::vector<std::string>{
                "slip_box",
                "simple_slip_box"
            }
        );

        REGISTER_MIGRATIONS(SlipBox, SQLite)

        REGISTER_MODEL(map, Map, MAP)
        REGISTER_MODEL(note, Note, NOTE)
        REGISTER_MODEL(content, Content, CONTENT)
        REGISTER_MODEL(source, Source, SOURCE)
        REGISTER_MODEL(tag, Tag, TAG)
        REGISTER_MODEL(property, Property, PROPERTY)

        REGISTER_MODEL(link, Link, LINK)
        REGISTER_MODEL(url, Url, URL)
        REGISTER_MODEL(wanted_note, WantedNote, WANTED_NOTE)

        REGISTER_MODEL(question, Question, QUESTION)
        REGISTER_MODEL(tag_type, TagType, TAG_TYPE)

        REGISTER_MODEL(collection, Collection, COLLECTION)
        REGISTER_MODEL(collection_item, CollectionItem, COLLECTION_ITEM)
        REGISTER_MODEL(term, Term, TERM)
        REGISTER_MODEL(idea, Idea, IDEA)
        REGISTER_MODEL(alert, Alert, ALERT)
        REGISTER_MODEL(flag, Flag, FLAG)
        REGISTER_MODEL(project, Project, PROJECT)
        REGISTER_MODEL(task, Task, TASK)
        REGISTER_MODEL(pinned_note, PinnedNote, PINNED_NOTE)
        REGISTER_MODEL(map_collection, MapCollection, MAP_COLLECTION)
        REGISTER_MODEL(map_collection_item, MapCollectionItem, MAP_COLLECTION_ITEM)
        REGISTER_MODEL(note_navigation, NoteNavigation, NOTE_NAVIGATION)
        REGISTER_MODEL(test, Test, TEST)
        REGISTER_MODEL(test_attempt, TestAttempt, TEST_ATTEMPT)
        REGISTER_MODEL(test_attempt_answer, TestAttemptAnswer, TEST_ATTEMPT_ANSWER)
        REGISTER_MODEL(annotation, Annotation, ANNOTATION)
        REGISTER_MODEL(term_fulltext, TermFulltext, TERM_FULLTEXT)
        REGISTER_MODEL(tag_type_fulltext, TagTypeFulltext, TAG_TYPE_FULLTEXT)

        plugin->register_trigger(std::make_shared<triggers::UpdateNotePathAndDepthAfterTrigger>());
        plugin->register_trigger(std::make_shared<triggers::AfterUpdateContentTrigger>());
        plugin->register_trigger(std::make_shared<triggers::AfterCreateUpdateNoteTrigger>());
        plugin->register_trigger(std::make_shared<triggers::BeforeCreateNoteTrigger>());
        plugin->register_trigger(std::make_shared<triggers::BeforeUpdateContentTrigger>());
        plugin->register_trigger(std::make_shared<triggers::InsteadOfReadNoteNavigationTrigger>());
        plugin->register_trigger(std::make_shared<triggers::AfterCreateTestAttemptTrigger>());
        plugin->register_trigger(std::make_shared<triggers::AfterCreateTestAttemptAnswerTrigger>());
        plugin->register_trigger(std::make_shared<triggers::InsteadOfListTermFulltextTrigger>());
        plugin->register_trigger(std::make_shared<triggers::InsteadOfListTagTypeFulltextTrigger>());

        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::UpdateNotePathAndDepthSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::FindNotesInMapSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::FindNextSiblingOrderSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::FindPreviousAndNextNoteSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::GetQuestionIdsSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::FindTermsSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::FindTagTypesSQLiteQuery>());

        plugin->register_job(std::make_shared<mindnet::plugins::dictionary::jobs::HtmlExportJob>());
        plugin->register_library_file("markdown-it.min.js");
        plugin->register_library_file("highlight.min.js");
        plugin->register_library_file("markdown-it-emoji.min.js");
        plugin->register_library_file("github.min.css");

        plugin->close_for_changes();
        return plugin;
    }
}