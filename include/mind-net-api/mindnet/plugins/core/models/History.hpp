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
#include "mindnet/model/BaseModel.hpp"

// ***** MACROS : START *****
#define Model History
#define MODEL HISTORY
#define COLS columns::HistoryColumns
#include "../columns/HistoryColumns.hpp"
#include "mindnet/plugins/core/CorePlugin.hpp"
#include "mindnet/plugins/core/enums/Crudl.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::core::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();
    inline const def HISTORY_DEFINITION =
        def(COLS::MODEL_NAME, CORE_PLUGIN_NAME)
        .set_rest_operations("rl")
        .set_group("Core", 500).allow_reader_write().set_cache_enabled(false)
        .set_columns({
            //
            coldef(COLS::USER_ID, FOREIGN_KEY | READONLY).set_description("User ID who made the change"),
            coldef(COLS::TABLE_NAME, MANDATORY | READONLY).set_description("Name of the table where change was made"),
            coldef(COLS::RECORD_ID, MANDATORY | READONLY).set_description("ID of the record that was changed"),
            coldef(COLS::OPERATION, MANDATORY | READONLY).set_enum_definition(
                mindnet::plugins::core::enums::crudl_to_enum_definition()).set_description(
                "Type of operation performed"),
            coldef(COLS::DATA_JSON, MANDATORY | READONLY).set_description("JSON data containing the changes"),
            coldef(COLS::REASON, READONLY).set_description("Reason for making the change"),
            //
        });

    struct Model : mindnet::model::BaseModel
    {
        identification user_id{};
        string table_name;
        identification record_id{};
        mindnet::essential::Crudl operation{};
        string data_json;
        string reason;

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                table_name == other.table_name &&
                record_id == other.record_id &&
                operation == other.operation &&
                data_json == other.data_json &&
                reason == other.reason;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS

