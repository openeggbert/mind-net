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
#define Model Error
#define MODEL ERROR
#define COLS columns::ErrorColumns
#include "../columns/ErrorColumns.hpp"
#include "mindnet/plugins/core/CorePlugin.hpp"
#include "mindnet/plugins/core/enums/Crudl.hpp"
#include "mindnet/plugins/core/enums/ErrorSeverity.hpp"
#include "mindnet/plugins/core/enums/ErrorOrigin.hpp"
#include "mindnet/plugins/core/enums/ExecutionLayer.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::core::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def ERROR_DEFINITION =
        def(COLS::MODEL_NAME, CORE_PLUGIN_NAME)
            .set_rest_operations("rul")
            .set_group("Core", 600)
            .set_columns({
                /* identity */
                coldef(COLS::PUBLIC_ID, MANDATORY | READONLY)
                    .set_description("Public error identifier shown to the user"),
                coldef(COLS::UUID, MANDATORY | READONLY)
                    .set_description("Internal unique error identifier"),

                /* classification */
                coldef(COLS::SEVERITY, MANDATORY | READONLY)
                    .set_enum_definition(
                        mindnet::plugins::core::enums::error_severity_to_enum_definition())
                    .set_description("Severity of the error"),
                coldef(COLS::HTTP_STATUS, INTEGER | READONLY)
                    .set_description("HTTP status associated with the error"),

                /* origin & layer */
                coldef(COLS::ORIGIN, READONLY)
                    .set_enum_definition(
                        mindnet::plugins::core::enums::error_origin_to_enum_definition())
                    .set_description("Execution origin that triggered the error"),
                coldef(COLS::LAYER, READONLY)
                    .set_enum_definition(
                        mindnet::plugins::core::enums::execution_layer_to_enum_definition())
                    .set_description("Build-time layer (CMake target) where the error occurred"),

                /* system location */
                coldef(COLS::PLUGIN, READONLY)
                    .set_description("Plugin in which the error occurred"),
                coldef(COLS::OPERATION, READONLY)
                    .set_enum_definition(enums::crudl_to_enum_definition())
                    .set_description("High-level operation being executed"),

                /* code location */
                coldef(COLS::CPP_NAMESPACE, READONLY)
                    .set_description("C++ namespace where the error occurred"),
                coldef(COLS::CPP_CLASS, READONLY)
                    .set_description("C++ class where the error occurred"),
                coldef(COLS::CPP_SYMBOL, READONLY)
                    .set_description("C++ function or method where the error occurred"),

                /* technical context */
                coldef(COLS::SQL_QUERY, READONLY)
                    .set_description("SQL query associated with the error, if any"),

                /* request / user context */
                coldef(COLS::USER_ID, INTEGER | READONLY)
                    .set_description("User associated with the error, if any"),
                coldef(COLS::REQUEST_ID, READONLY)
                    .set_description("Request correlation identifier"),

                /* payload */
                coldef(COLS::MESSAGE, MANDATORY | READONLY)
                    .set_description("Human-readable error message"),
                coldef(COLS::EXCEPTION_TYPE, READONLY)
                    .set_description("C++ exception type"),
                   coldef(COLS::EXCEPTION_MESSAGE, READONLY),

                /* lifecycle */
                coldef(COLS::HANDLED, BOOL)
                    .set_description("Error handling state"),

                /* versioning */
                coldef(COLS::MINDNET_VERSION, READONLY)
                    .set_description("Mind Net version in which the error occurred"),
            });

    struct Model : mindnet::model::BaseModel
    {
        std::string public_id;
        std::string uuid;

        mindnet::plugins::core::enums::ErrorSeverity severity{};
        int http_status{};

        mindnet::plugins::core::enums::ErrorOrigin origin{};
        mindnet::plugins::core::enums::ExecutionLayer layer{};

        std::string plugin;
        essential::Crudl operation;

        std::string cpp_namespace;
        std::string cpp_class;
        std::string cpp_symbol;

        std::string sql_query;

        identification user_id{};
        std::string request_id;

        std::string message;
        std::string exception_type;
        std::string exception_message;

        bool handled{false};

        std::string mindnet_version;

        static constexpr auto fields = std::make_tuple(
            &Model::public_id,
            &Model::uuid,

            &Model::severity,
            &Model::http_status,

            &Model::origin,
            &Model::layer,

            &Model::plugin,
            &Model::operation,

            &Model::cpp_namespace,
            &Model::cpp_class,
            &Model::cpp_symbol,

            &Model::sql_query,

            &Model::user_id,
            &Model::request_id,

            &Model::message,
            &Model::exception_type,
            &Model::exception_message,

            &Model::handled,

            &Model::mindnet_version
        );

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                   created_at == other.created_at &&
                   updated_at == other.updated_at &&
                   public_id == other.public_id &&
                   uuid == other.uuid &&
                   severity == other.severity &&
                   http_status == other.http_status &&
                   origin == other.origin &&
                   layer == other.layer &&
                   plugin == other.plugin &&
                   operation == other.operation &&
                   cpp_namespace == other.cpp_namespace &&
                   cpp_class == other.cpp_class &&
                   cpp_symbol == other.cpp_symbol &&
                   sql_query == other.sql_query &&
                   user_id == other.user_id &&
                   request_id == other.request_id &&
                   message == other.message &&
                   exception_type == other.exception_type &&
                   exception_message == other.exception_message &&
                   handled == other.handled &&
                   mindnet_version == other.mindnet_version;
        }
    };

}

#undef Model
#undef MODEL
#undef COLS
