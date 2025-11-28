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
#include "mindnet/plugins/core/CorePlugin.hpp"

// ***** MACROS : START *****
#define Model Team
#define MODEL TEAM
#define COLS columns::TeamColumns
#include "../columns/TeamColumns.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::core::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def TEAM_DEFINITION =
        def(COLS::MODEL_NAME, CORE_PLUGIN_NAME).set_rest_operations("rl")
                                               .set_group("Core", 300)
                                               .set_title_column(COLS::NAME)
                                               .set_columns({
                                                   //
                                                   coldef(COLS::NAME, MANDATORY | UNIQUE),
                                                   coldef(COLS::DESCRIPTION),
                                                   coldef(COLS::CREATED_BY, READONLY | MANDATORY).set_foreign_key(
                                                       "user"),
                                                   coldef(COLS::LEADER_ID, MANDATORY).set_foreign_key("user"),

                                                   //
                                               });

    struct Model : mindnet::model::BaseModel
    {
        string name;
        string description;
        identification created_by{};
        identification leader_id{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                name == other.name &&
                description == other.description &&
                created_by == other.created_by &&
                leader_id == other.leader_id;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS