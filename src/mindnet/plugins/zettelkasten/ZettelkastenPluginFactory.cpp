

#include "mindnet/plugins/zettelkasten/ZettelkastenPluginFactory.h"
#include "mindnet/impl/sqlite/Convertors.h"
#include "mindnet/plugins/zettelkasten/validators/CollectionValidator.h"
#include "mindnet/plugins/zettelkasten/validators/CollectionItemValidator.h"
#include "mindnet/plugins/zettelkasten/validators/ContentValidator.h"
#include "mindnet/plugins/zettelkasten/validators/LinkValidator.h"
#include "mindnet/plugins/zettelkasten/validators/MapValidator.h"
#include "mindnet/plugins/zettelkasten/validators/NoteValidator.h"
#include "mindnet/plugins/zettelkasten/validators/PropertyValidator.h"
#include "mindnet/plugins/zettelkasten/validators/QuestionValidator.h"
#include "mindnet/plugins/zettelkasten/validators/ReferenceValidator.h"
#include "mindnet/plugins/zettelkasten/validators/TagValidator.h"
#include "mindnet/plugins/zettelkasten/validators/TagTypeValidator.h"

namespace mindnet::plugins::zettelkasten
{
    api::PluginPtr ZettelkastenPluginFactory::create() const
    {
        auto plugin = std::make_shared<api::Plugin>(
            "zettelkasten",
            "zettelkasten",
            std::vector<std::string>{},
            false
        );
        REGISTER_MODEL(collection, Collection, COLLECTION)
        REGISTER_MODEL(collection_item, CollectionItem, COLLECTION_ITEM)
        REGISTER_MODEL(content, Content, CONTENT)
        REGISTER_MODEL(link, Link, LINK)
        REGISTER_MODEL(map, Map, MAP)
        REGISTER_MODEL(note, Note, NOTE)
        REGISTER_MODEL(tag, Tag, TAG)
        REGISTER_MODEL(property, Property, PROPERTY)

        REGISTER_MODEL(reference, Reference, REFERENCE)

        REGISTER_MODEL(question, Question, QUESTION)
        REGISTER_MODEL(tag_type, TagType, TAG_TYPE)

        plugin->close_for_changes();
        return plugin;
    }
}
