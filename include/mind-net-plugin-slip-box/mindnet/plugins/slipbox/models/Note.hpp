///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
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

#include "mindnet/model/BaseModel.hpp"
// ***** MACROS : START *****
#define Model Note
#define MODEL NOTE
#define COLS columns::NoteColumns
#include "../columns/NoteColumns.hpp"
// ***** MACROS : END *****

#include "../enums/Difficulty.hpp"
#include "../enums/Importance.hpp"

/**
 * @file Note.h
 * @brief Definition of the SlipBox Note model within Mind Net ORM framework.
 *
 * The Note entity represents a single atomic note in the SlipBox (Zettelkasten) system.
 * Each note may have hierarchical relationships, metadata, and references to content.
 *
 * ### Core characteristics
 * - Belongs to a map (`map_id`)
 * - May have a parent note (`parent_note_id`)
 * - References one content record (`content_id`)
 * - May reference a source (`source_id`) or be an alias for another note (`alias_for_note_id`)
 *
 * ### Semantic fields
 * - `title` — Human-readable title of the note.
 * - `hint` — Short recall question or guidance text (used for spaced repetition / review sessions).
 *             Limited to ~128 characters by the ORM validation layer.
 * - `importance` — Enum defining note's significance in the knowledge structure.
 * - `difficulty` — Enum defining how difficult the note is to recall or understand.
 *
 * ### Structural fields
 * - `sibling_order` — Relative order among siblings within the same parent.
 * - `path` — Internal materialized path for hierarchical queries.
 * - `depth` — Depth level of the note within the hierarchy.
 *
 * ### Related entities
 * The model automatically exposes custom REST actions to interact with related entities:
 * - `tag`, `property`, `url`, `link`
 *
 * ### Example usage
 * ```cpp
 * mindnet::plugins::slipbox::models::Note note;
 * note.title = "Type conversions in C++";
 * note.hint = "What kinds of type conversions does C++ have?";
 * note.importance = enums::Importance::Medium;
 * note.difficulty = enums::Difficulty::Easy;
 * orm.save(note);
 * ```
 */

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def NOTE_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_group("Slip Box", 100)
        .set_all_rest_operations().set_title_column(COLS::TITLE)
        .set_columns({
            //
            coldef(COLS::MAP_ID, MANDATORY | FOREIGN_KEY | READONLY).set_description("Map, this note belongs to."),
            coldef(COLS::PARENT_NOTE_ID).set_foreign_key("note").set_description("Parent note, if any."),
            coldef(COLS::CONTENT_ID, FOREIGN_KEY | UNIQUE).set_description("Content associated with this note."),
            coldef(COLS::SOURCE_ID, FOREIGN_KEY).set_description("Source associated with this note."),
            coldef(COLS::ALIAS_FOR_NOTE_ID).set_foreign_key("note").
                                            set_description("Source associated with this note."),
            coldef(COLS::TITLE, MANDATORY).set_description("Title of the note."),
            coldef(COLS::HINT).set_description("short recall question or guidance text (max ~128 chars)"),
            coldef(COLS::SIBLING_ORDER, INTEGER | AUTO).set_description("Order among sibling notes."),
            coldef(COLS::IMPORTANCE).set_default_value(0).set_enum_definition(enums::importance_to_enum_definition()).
                                     set_description("Importance level of the note."),
            coldef(COLS::DIFFICULTY).set_default_value(0).set_enum_definition(enums::difficulty_to_enum_definition()).
                                     set_description("Difficulty level of the note."),
            coldef(COLS::PATH, INTERNAL),
            coldef(COLS::DEPTH, INTEGER | INTERNAL),
        })
        .add_custom_list_action("note", "List children", {"parent_note_id", "{id}"})
        .add_custom_create_action("note", "Add child", {"parent_note_id", "{id}"})
        .add_custom_list_action("tag", "List tags", {"note_id", "{id}"})
        .add_custom_create_action("tag", "Add tag", {"note_id", "{id}"})
        .add_custom_list_action("property", "List properties", {"note_id", "{id}"})
        .add_custom_create_action("property", "Add property", {"note_id", "{id}"})
        .add_custom_list_action("url", "List urls", {"from_note_id", "{id}"})
        .add_custom_create_action("url", "Add url", {"from_note_id", "{id}"})
        .add_custom_list_action("link", "List links", {"from_note_id", "{id}"})
        .add_custom_create_action("link", "Add link", {"from_note_id", "{id}"});

    struct Model : mindnet::model::BaseModel
    {
        i64 map_id{};
        i64 parent_note_id{};
        i64 content_id{};
        i64 source_id{};
        i64 alias_for_note_id{};
        string title;
        string hint;
        int sibling_order{};
        enums::Importance importance{enums::Importance::Undefined};
        enums::Difficulty difficulty{enums::Difficulty::Undefined};
        string path;
        int depth{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id && map_id == other.map_id &&
                sibling_order == other.sibling_order && title == other.title &&
                hint == other.hint &&
                content_id == other.content_id && source_id == other.source_id &&
                alias_for_note_id == other.alias_for_note_id &&
                parent_note_id == other.parent_note_id &&
                importance == other.importance && difficulty == other.difficulty &&
                path == other.path && depth == other.depth &&
                created_at == other.created_at && updated_at == other.updated_at;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // NOTE_H
