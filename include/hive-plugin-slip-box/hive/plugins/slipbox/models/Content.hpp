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

#include "hive/model/BaseModel.hpp"
#include "../columns/ContentColumns.hpp"
#include "hive/essential/DatabaseType.hpp"
#include "../enums/ContentFormat.hpp"

// ***** MACROS : START *****
#define Model Content
#define MODEL CONTENT
#define COLS columns::ContentColumns
#include "../columns/ContentColumns.hpp"
#include "hive/plugins/slipbox/SlipBoxPlugin.hpp"

// ***** MACROS : END *****

namespace hive::plugins::slipbox::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();

    inline const def CONTENT_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_all_rest_operations()
        .set_group("Other", 500)
        .set_group("Slip Box", 100)
        .set_columns({
            //
            coldef(COLS::VALUE, TEXTAREA | MANDATORY).set_description("Content value/body"),
            coldef(COLS::FORMAT).set_default_value(0).set_enum_definition(
                enums::content_format_to_enum_definition()).set_description("Content format type"),
            coldef(COLS::VERSION, INTEGER).set_default_value("1").set_description("Content version number"),
            coldef(COLS::SEMANTIC_VERSION, INTEGER).set_default_value(1),
            coldef(COLS::CHANGE_RATIO, INTEGER).set_default_value(0),
            coldef(COLS::LAST_PARSED_SUCCESS_AT, DATETIME).set_default_value(0),
            coldef(COLS::LAST_PARSED_FAIL_AT, DATETIME).set_default_value(0),
            //
        })
        .add_custom_list_action("note", "Get note", {"content_id", "{id}"});

    struct Model : hive::model::BaseModel
    {
        // definition, example, note
        string value;
        enums::ContentFormat format{};
        int version{1};
        int semantic_version{1};
        int change_ratio{0};
        unixtime last_parsed_success_at{};
        unixtime last_parsed_fail_at{};

        static constexpr auto fields = std::make_tuple(
            &Model::value,
            &Model::format,
            &Model::version,
            &Model::semantic_version,
            &Model::change_ratio,
            &Model::last_parsed_success_at,
            &Model::last_parsed_fail_at
        );

        create_model_h_methods(Model, MODEL)

    };
}

#undef Model
#undef MODEL
#undef COLS