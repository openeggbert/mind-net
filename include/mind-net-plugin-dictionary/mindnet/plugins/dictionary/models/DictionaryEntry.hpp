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

// ***** MACROS : START *****
#define Model DictionaryEntry
#define MODEL DICTIONARY_ENTRY
#define COLS columns::DictionaryEntryColumns
#include "../columns/DictionaryEntryColumns.hpp"
// ***** MACROS : END *****

#include "../enums/Difficulty.hpp"

namespace mindnet::plugins::dictionary::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def DICTIONARY_ENTRY_DEFINITION =
        def(COLS::MODEL_NAME, "dictionary")
        .set_group("Dictionary", 200)
        .set_all_rest_operations().set_title_column(COLS::TITLE)
        .set_columns({
            //
            coldef(COLS::DICTIONARY_MAP_ID, MANDATORY | FOREIGN_KEY | READONLY).set_description("Map, this entry belongs to."),
            coldef(COLS::TITLE, MANDATORY).set_description("Title of the entry."),
            coldef(COLS::DEFINITION, TEXTAREA),
            coldef(COLS::DIFFICULTY).set_default_value(0).set_enum_definition(enums::difficulty_to_enum_definition()).
                                     set_description("Difficulty level of the note."),
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