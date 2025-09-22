

#include "mindnet/other/plugins/slipbox/SlipBoxPluginFactory.h"
#include "mindnet/other/plugins/slipbox/validators/CollectionValidator.h"
#include "mindnet/other/plugins/slipbox/validators/CollectionItemValidator.h"
#include "mindnet/other/plugins/slipbox/validators/ContentValidator.h"
#include "mindnet/other/plugins/slipbox/validators/LinkValidator.h"
#include "mindnet/other/plugins/slipbox/validators/MapValidator.h"
#include "mindnet/other/plugins/slipbox/validators/NoteValidator.h"
#include "mindnet/other/plugins/slipbox/validators/PropertyValidator.h"
#include "mindnet/other/plugins/slipbox/validators/QuestionValidator.h"
#include "mindnet/other/plugins/slipbox/validators/ReferenceValidator.h"
#include "mindnet/other/plugins/slipbox/validators/TagValidator.h"
#include "mindnet/other/plugins/slipbox/validators/TagTypeValidator.h"
#include "mindnet/other/plugins/slipbox/validators/ConceptValidator.h"
#include "mindnet/other/plugins/slipbox/validators/SourceValidator.h"
#include "mindnet/other/plugins/slipbox/validators/IdeaValidator.h"
#include "mindnet/other/plugins/slipbox/validators/WantedNoteValidator.h"
#include "mindnet/other/plugins/slipbox/migrations/SlipBoxSQLiteMigrationScripts.h"

namespace mindnet::plugins::slipbox
{
    const string SLIP_BOX_PLUGIN_NAME = "slip_box";
    api::PluginPtr SlipBoxPluginFactory::create() const
    {
        auto plugin = std::make_shared<api::Plugin>(
            SLIP_BOX_PLUGIN_NAME,
            "slip_box",
            std::vector<std::string>{"slip_box"}
        );

        REGISTER_MIGRATIONS(SlipBox, SQLite)




        REGISTER_MODEL(map, Map, MAP)
        REGISTER_MODEL(note, Note, NOTE)
        REGISTER_MODEL(content, Content, CONTENT)
        REGISTER_MODEL(source, Source, SOURCE)
        REGISTER_MODEL(tag, Tag, TAG)
        REGISTER_MODEL(property, Property, PROPERTY)

        REGISTER_MODEL(reference, Reference, REFERENCE)
        REGISTER_MODEL(link, Link, LINK)
        REGISTER_MODEL(wanted_note, WantedNote, WANTED_NOTE)

        REGISTER_MODEL(question, Question, QUESTION)
        REGISTER_MODEL(tag_type, TagType, TAG_TYPE)

        REGISTER_MODEL(collection, Collection, COLLECTION)
        REGISTER_MODEL(collection_item, CollectionItem, COLLECTION_ITEM)
        REGISTER_MODEL(concept, Concept, CONCEPT)
        REGISTER_MODEL(idea, Idea, IDEA)


        plugin->close_for_changes();
        return plugin;
    }
}
