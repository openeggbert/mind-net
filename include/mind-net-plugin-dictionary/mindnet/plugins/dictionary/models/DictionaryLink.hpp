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
#include "mindnet/plugins/dictionary/DictionaryPlugin.hpp"
// ***** MACROS : START *****
#define Model DictionaryLink
#define MODEL DICTIONARY_LINK
#define COLS columns::DictionaryLinkColumns
#include "../columns/DictionaryLinkColumns.hpp"
#include "mindnet/plugins/dictionary/enums/TermRelation.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::dictionary::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def DICTIONARY_LINK_DEFINITION =
        def(COLS::MODEL_NAME, DICTIONARY_PLUGIN_NAME)
        .set_all_rest_operations()
        .set_group("Dictionary", 100)
        .set_columns({
            coldef(COLS::FROM_DICTIONARY_TERM_ID, MANDATORY | READONLY).set_foreign_key("dictionary_term"),
            coldef(COLS::TO_DICTIONARY_TERM_ID, MANDATORY | READONLY).set_foreign_key("dictionary_term"),
            coldef(COLS::TYPE).set_default_value(0).set_enum_definition(enums::term_relation_type_enum_definition())
        });

    struct Model : mindnet::model::BaseModel
    {
        identification from_dictionary_term_id;
        identification to_dictionary_term_id;
        enums::TermRelationType type{0};

        static constexpr auto fields = std::make_tuple(
            &Model::from_dictionary_term_id,
            &Model::to_dictionary_term_id,
            &Model::type
        );

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                from_dictionary_term_id == other.from_dictionary_term_id &&
                to_dictionary_term_id == other.to_dictionary_term_id &&
                type == other.type &&
                created_at == other.created_at &&
                updated_at == other.updated_at;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
