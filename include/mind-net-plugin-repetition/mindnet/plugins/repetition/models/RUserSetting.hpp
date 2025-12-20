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
#define Model RUserSetting
#define MODEL R_USER_SETTING
#define COLS columns::RUserSettingColumns
#include "../columns/RUserSettingColumns.hpp"
#include "mindnet/plugins/repetition/RepetitionPluginFactory.hpp"
#include "mindnet/plugins/repetition/RepetitionPlugin.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::repetition::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def R_USER_SETTING_DEFINITION =
        def(COLS::MODEL_NAME, REPETITION_PLUGIN_NAME)
        .set_all_rest_operations()
        .set_group("Repetition", 200)
        .set_title_column(COLS::KEY)
        .allow_reader_write()
        .set_columns({
            coldef(COLS::USER_ID, FOREIGN_KEY | MANDATORY),
            coldef(COLS::KEY, MANDATORY | UNIQUE),
            coldef(COLS::VALUE, MANDATORY),
        });

    struct Model : mindnet::model::BaseModel
    {
        identification user_id{};
        string key;
        string value;

        static constexpr auto fields = std::make_tuple(
            &Model::user_id,
            &Model::key,
            &Model::value
        );

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                key == other.key &&
                value == other.value;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS