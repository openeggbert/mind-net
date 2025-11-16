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
#pragma once


#include <string>

#include "mindnet/model/BaseModel.hpp"
#include "../columns/ContentColumns.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "../enums/ContentFormat.hpp"

// ***** MACROS : START *****
#define Model Content
#define MODEL CONTENT
#define COLS columns::ContentColumns
#include "../columns/ContentColumns.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def CONTENT_DEFINITION =
        def(COLS::MODEL_NAME)
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

    struct Model : mindnet::model::BaseModel
    {
        // definition, example, note
        string value;
        enums::ContentFormat format{};
        int version{1};
        int semantic_version{1};
        int change_ratio{0};
        unixtime last_parsed_success_at{};
        unixtime last_parsed_fail_at{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                value == other.value &&
                format == other.format &&
                version == other.version &&
                semantic_version == other.semantic_version &&
                change_ratio == other.change_ratio &&
                last_parsed_success_at == other.last_parsed_success_at &&
                last_parsed_fail_at == other.last_parsed_fail_at;
        }
    };
}

#undef Model
#undef MODEL
#undef COLS
