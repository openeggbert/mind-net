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

// ***** MACROS : START *****
#define Model History
#define MODEL HISTORY
#define COLS columns::HistoryColumns
#include "../columns/HistoryColumns.hpp"
#include "hive/plugins/core/CorePlugin.hpp"
#include "hive/plugins/core/enums/Crudl.hpp"

// ***** MACROS : END *****

namespace hive::plugins::core::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();
    inline const def HISTORY_DEFINITION =
        def(COLS::MODEL_NAME, CORE_PLUGIN_NAME)
        .set_rest_operations("rl")
        .set_group("Core", 500).allow_reader_write().set_cache_enabled(false).set_readonly()
        .set_columns({
            //
            coldef(COLS::USER_ID, FOREIGN_KEY).set_description("User ID who made the change"),
            coldef(COLS::TABLE_NAME, MANDATORY).set_description("Name of the table where change was made"),
            coldef(COLS::RECORD_ID, MANDATORY).set_description("ID of the record that was changed"),
            coldef(COLS::OPERATION, MANDATORY).set_enum_definition(
                hive::plugins::core::enums::crudl_to_enum_definition()).set_description(
                "Type of operation performed"),
            coldef(COLS::DATA_JSON, MANDATORY).set_description("JSON data containing the changes"),
            coldef(COLS::REASON).set_description("Reason for making the change"),
            //
        });

    struct Model : hive::model::BaseModel
    {
        identification user_id{};
        string table_name;
        identification record_id{};
        hive::essential::Crudl operation{};
        string data_json;
        string reason;

        static constexpr auto fields = std::make_tuple(
            &Model::user_id,
            &Model::table_name,
            &Model::record_id,
            &Model::operation,
            &Model::data_json,
            &Model::reason
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS