#include "mindnet/plugins/slipbox/SlipBoxPluginFactory.h"

#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindNextSiblingOrderSQLiteQuery.h"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindNotesInMapSQLiteQuery.h"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/GetRSessionSelectedItemsSQLiteQuery.h"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/UpdateNotePathAndDepthSQLiteQuery.h"
#include "mindnet/api/Plugin.h"
#include "mindnet/api/PluginFactory.h"
#include "mindnet/plugins/slipbox/validators/CollectionValidator.h"
#include "mindnet/plugins/slipbox/validators/CollectionItemValidator.h"
#include "mindnet/plugins/slipbox/validators/ContentValidator.h"
#include "mindnet/plugins/slipbox/validators/UrlValidator.h"
#include "mindnet/plugins/slipbox/validators/MapValidator.h"
#include "mindnet/plugins/slipbox/validators/NoteValidator.h"
#include "mindnet/plugins/slipbox/validators/PropertyValidator.h"
#include "mindnet/plugins/slipbox/validators/QuestionValidator.h"
#include "mindnet/plugins/slipbox/validators/LinkValidator.h"
#include "mindnet/plugins/slipbox/validators/TagValidator.h"
#include "mindnet/plugins/slipbox/validators/TagTypeValidator.h"
#include "mindnet/plugins/slipbox/validators/TermValidator.h"
#include "mindnet/plugins/slipbox/validators/SourceValidator.h"
#include "mindnet/plugins/slipbox/validators/IdeaValidator.h"
#include "mindnet/plugins/slipbox/validators/WantedNoteValidator.h"
#include "mindnet/plugins/slipbox/validators/AlertValidator.h"
#include "mindnet/plugins/slipbox/validators/FlagValidator.h"
#include "mindnet/plugins/slipbox/validators/ProjectValidator.h"
#include "mindnet/plugins/slipbox/validators/TaskValidator.h"
#include "mindnet/plugins/slipbox/validators/PinnedNoteValidator.h"
#include "mindnet/plugins/slipbox/migrations/SlipBoxSQLiteMigrationScripts.h"
#include "mindnet/plugins/slipbox/triggers/AfterCreateUpdateNoteTrigger.h"
#include "mindnet/plugins/slipbox/triggers/AfterUpdateContentTrigger.h"
#include "mindnet/plugins/slipbox/triggers/BeforeCreateNoteTrigger.h"
#include "mindnet/plugins/slipbox/triggers/UpdateNotePathAndDepthAfterTrigger.h"

namespace mindnet::plugins::slipbox
{
    const string SLIP_BOX_PLUGIN_NAME = "slip_box";

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

        plugin->register_trigger(std::make_shared<triggers::UpdateNotePathAndDepthAfterTrigger>());
        plugin->register_trigger(std::make_shared<triggers::AfterUpdateContentTrigger>());
        plugin->register_trigger(std::make_shared<triggers::AfterCreateUpdateNoteTrigger>());
        plugin->register_trigger(std::make_shared<triggers::BeforeCreateNoteTrigger>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::UpdateNotePathAndDepthSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::FindNotesInMapSQLiteQuery>());
        plugin->register_query(std::make_shared<mindnet::db::sqlite::queries::FindNextSiblingOrderSQLiteQuery>());
        plugin->register_library_file("markdown-it.min.js");
        plugin->register_library_file("highlight.min.js");
        plugin->register_library_file("markdown-it-emoji.min.js");
        plugin->register_library_file("github.min.css");

        plugin->close_for_changes();
        return plugin;
    }
}
