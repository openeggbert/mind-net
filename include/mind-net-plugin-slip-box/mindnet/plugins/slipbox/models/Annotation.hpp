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
#include "mindnet/plugins/slipbox/SlipBoxPlugin.hpp"
#include "../enums/AnnotationType.hpp"

// ***** MACROS : START *****
#define Model Annotation
#define MODEL ANNOTATION
#define COLS columns::AnnotationColumns
#include "../columns/AnnotationColumns.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def ANNOTATION_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_rest_operations("crudl")
        .set_group("Slip Box #2", 110)
        .set_columns({
            coldef(COLS::NOTE_ID, MANDATORY | FOREIGN_KEY | READONLY),
            coldef(COLS::PARENT_ANNOTATION_ID).set_foreign_key("annotation"),
            coldef(COLS::TITLE),
            coldef(COLS::CONTENT, MANDATORY),
            coldef(COLS::TYPE, MANDATORY).set_enum_definition(enums::annotation_type_to_enum_definition()),
            coldef(COLS::ANCHOR_START_TEXT, MANDATORY),
            coldef(COLS::ANCHOR_END_TEXT, MANDATORY),
        });
    ;

    struct Model : mindnet::model::BaseModel
    {
        identification note_id;
        identification parent_annotation_id;
        std::string title;
        std::string content;
        enums::AnnotationType type;
        std::string anchor_start_text;
        std::string anchor_end_text;

        static constexpr auto fields = std::make_tuple(
            &Model::note_id,
            &Model::parent_annotation_id,
            &Model::title,
            &Model::content,
            &Model::type,
            &Model::anchor_start_text,
            &Model::anchor_end_text
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS