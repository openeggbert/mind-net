///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : Mind map software.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see
// <https://www.gnu.org/licenses/> or write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////

/**
 *
 * @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */

#include <string>

#include "mindnet/enums/Crudl.h"
#include "mindnet/persistence/api/IRepository.h"
#include "mindnet/persistence/impl/sqlite/RepositoryHelper.h"
#include "mindnet/models/columns/UserColumns.h"
#include "mindnet/models/columns/HistoryColumns.h"
#include "mindnet/models/columns/MapColumns.h"
#include "mindnet/models/columns/NoteColumns.h"
#include "mindnet/models/columns/ContentColumns.h"
#include "mindnet/models/columns/PropertyColumns.h"
#include "mindnet/models/columns/TagTypeColumns.h"
#include "mindnet/models/columns/TagColumns.h"
#include "mindnet/models/columns/ReferenceColumns.h"
#include "mindnet/models/columns/LinkColumns.h"
#include "mindnet/persistence/impl/sqlite/repositories/Convertors.h"

namespace mindnet::persistence::impl::sqlite::repositories
{
    entity_fields convert_crow_json_rvalue_to_entity_fields_user(
        crow::json::rvalue& body, enums::Crudl crudl)
    {
        start_of_convert_crow_json_rvalue_to_entity_fields(User)

        add_string(USERNAME);
        add_string(PASSWORD_HASH);
        add_optional_string(DISPLAY_NAME, "");
        add_int(ROLE);
        add_optional_string(PROFILE_TEXT, "");
        add_optional_int(LAST_LOGIN, 0);
        add_optional_string(EMAIL, "");
        add_int(STATUS);
        return fields;
    }

    entity_fields convert_crow_json_rvalue_to_entity_fields_history(method_arguments())
    {
        start_of_convert_crow_json_rvalue_to_entity_fields(History)

        add_string(TABLE_NAME);
        add_int(RECORD_ID);
        add_int(OPERATION);
        add_string(DATA_JSON);
        add_optional_string(REASON, "");
        return fields;
    }

    entity_fields convert_crow_json_rvalue_to_entity_fields_map(method_arguments())
    {
        start_of_convert_crow_json_rvalue_to_entity_fields(Map)

        add_string(NAME);
        add_optional_string(DESCRIPTION, "");
        add_optional_string(CATEGORY, "");
        add_int_foreign_key(OWNER_ID);
        add_int_foreign_key(TEAM_ID);
        add_int(OWNER_RIGHTS);
        add_int(TEAM_RIGHTS);
        add_int(OTHER_RIGHTS);
        return fields;
    }

    entity_fields convert_crow_json_rvalue_to_entity_fields_note(method_arguments())
    {
        start_of_convert_crow_json_rvalue_to_entity_fields(Note)

        add_int(MAP_ID);
        add_int(SIBLING_POSITION);
        add_string(TITLE);

        add_int_foreign_key(CONTENT_ID)
        add_int_foreign_key(PARENT_NOTE_ID)

        add_optional_int(IMPORTANCE, 0);
        add_optional_int(DIFFICULTY, 0);
        return fields;
    }

    entity_fields convert_crow_json_rvalue_to_entity_fields_content(method_arguments())
    {
        start_of_convert_crow_json_rvalue_to_entity_fields(Content)

        add_string(VALUE);
        add_int(FORMAT);
        add_int(VERSION);

        return fields;
    }

    entity_fields convert_crow_json_rvalue_to_entity_fields_property(method_arguments())
    {
        start_of_convert_crow_json_rvalue_to_entity_fields(Property)

        add_int(MAP_ID);
        add_int(NOTE_ID);
        add_string(KEY);
        add_optional_string(VALUE, "");
        return fields;
    }

    entity_fields convert_crow_json_rvalue_to_entity_fields_tag_type(method_arguments())
    {
        start_of_convert_crow_json_rvalue_to_entity_fields(TagType)

        add_int(MAP_ID);
        add_string(TITLE);
        return fields;
    }

    entity_fields convert_crow_json_rvalue_to_entity_fields_tag(method_arguments())
    {
        start_of_convert_crow_json_rvalue_to_entity_fields(Tag)

        add_int(NOTE_ID);
        add_int(TAG_TYPE_ID);
        return fields;
    }

    entity_fields convert_crow_json_rvalue_to_entity_fields_reference(method_arguments())
    {
        start_of_convert_crow_json_rvalue_to_entity_fields(Reference)

        add_int(FROM_NOTE_ID);
        add_int(TO_NOTE_ID);
        add_optional_string(LABEL, "");
        return fields;
    }

    entity_fields convert_crow_json_rvalue_to_entity_fields_link(method_arguments())
    {
        start_of_convert_crow_json_rvalue_to_entity_fields(Link)

        add_int(FROM_NOTE_ID);
        add_string(TO_URL);
        return fields;
    }
}
