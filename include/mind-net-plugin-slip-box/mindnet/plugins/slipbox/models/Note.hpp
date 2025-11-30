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

#pragma once

#include <string>
#include <utility>

#include "mindnet/model/BaseModel.hpp"
// #include "mindnet/plugins/slipbox/SlipBoxPlugin.hpp"
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
        // def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        def(COLS::MODEL_NAME, "slip_box")
        .set_group("Slip Box", 100)
        .set_all_rest_operations().set_title_column(COLS::TITLE)
        .set_cached_after_create(false)
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
        .add_custom_create_action("link", "Add link", {"from_note_id", "{id}"})
        .add_custom_read_action("note_navigation", "Read Note Navigation", {"id", "{id}"});

    struct Model : mindnet::model::BaseModel
    {
        identification map_id{};
        identification parent_note_id{};
        identification content_id{};
        identification source_id{};
        identification alias_for_note_id{};
        string title;
        string hint;
        int sibling_order{};
        enums::Importance importance{enums::Importance::Undefined};
        enums::Difficulty difficulty{enums::Difficulty::Undefined};
        string path;
        int depth{};

        static constexpr auto fields = std::make_tuple(
            &Model::id,
            &Model::created_at,
            &Model::updated_at,

            &Model::map_id,
            &Model::parent_note_id,
            &Model::content_id,
            &Model::source_id,
            &Model::alias_for_note_id,

            &Model::title,
            &Model::hint,
            &Model::sibling_order,

            &Model::importance,
            &Model::difficulty,

            &Model::path,
            &Model::depth
        );

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