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
#ifndef NOTE_H
#define NOTE_H

#include <string>
#include <utility>

#include "../../../model/BaseModel.h"
// ***** MACROS : START *****
#define Model Note
#define MODEL NOTE
#define COLS columns::NoteColumns
#include "../columns/NoteColumns.h"
// ***** MACROS : END *****

#include "../enums/Difficulty.h"
#include "../enums/Importance.h"

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def NOTE_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations().set_title_column(COLS::TITLE)
        .set_columns({
            //
            coldef(COLS::MAP_ID, MANDATORY | FOREIGN_KEY | READONLY).set_description("Map, this note belongs to."),
            coldef(COLS::PARENT_NOTE_ID).set_foreign_key("note").set_description("Parent note, if any."),
            coldef(COLS::CONTENT_ID, FOREIGN_KEY | UNIQUE).set_description("Content associated with this note."),
            coldef(COLS::SOURCE_ID, FOREIGN_KEY).set_description("Source associated with this note."),
            coldef(COLS::TITLE, MANDATORY).set_description("Title of the note."),
            coldef(COLS::SIBLING_ORDER, INTEGER | AUTO).set_description("Order among sibling notes."),
            coldef(COLS::IMPORTANCE).set_default_value(0).set_enum_definition(enums::importance_to_enum_definition()).
                                     set_description("Importance level of the note."),
            coldef(COLS::DIFFICULTY).set_default_value(0).set_enum_definition(enums::difficulty_to_enum_definition()).
                                     set_description("Difficulty level of the note.")
        })
        .add_custom_list_action("note", "List children", {"parent_note_id", "{id}"})
        .add_custom_create_action("note", "Add child", {"parent_note_id", "{id}"})
        .add_custom_list_action("tag", "List tags", {"note_id", "{id}"})
        .add_custom_create_action("tag", "Add tag", {"note_id", "{id}"})
        .add_custom_list_action("property", "List properties", {"note_id", "{id}"})
        .add_custom_create_action("property", "Add property", {"note_id", "{id}"})
        .add_custom_list_action("reference", "List references", {"from_note_id", "{id}"})
        .add_custom_create_action("reference", "Add reference", {"from_note_id", "{id}"})
        .add_custom_list_action("link", "List links", {"from_note_id", "{id}"})
        .add_custom_create_action("link", "Add link", {"from_note_id", "{id}"});

    struct Model : mindnet::model::BaseModel
    {
        int map_id{};
        int parent_note_id{};
        int content_id{};
        int source_id{};
        string title;
        int sibling_order{};
        enums::Importance importance{enums::Importance::Undefined};
        enums::Difficulty difficulty{enums::Difficulty::Undefined};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id && map_id == other.map_id &&
                sibling_order == other.sibling_order && title == other.title &&
                content_id == other.content_id && source_id== other.source_id &&
                    parent_note_id == other.parent_note_id &&
                importance == other.importance && difficulty == other.difficulty &&
                created_at == other.created_at && updated_at == other.updated_at;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // NOTE_H
