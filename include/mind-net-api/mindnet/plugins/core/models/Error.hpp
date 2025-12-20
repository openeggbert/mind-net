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
#define ModelERROR Error
#define MODELERROR ERROR
#define COLSERROR columns::ErrorColumns
#include "../columns/ErrorColumns.hpp"
#include "mindnet/plugins/core/CorePlugin.hpp"
#include "mindnet/plugins/core/enums/Crudl.hpp"
#include "mindnet/plugins/core/enums/ErrorSeverity.hpp"
#include "mindnet/plugins/core/enums/ErrorOrigin.hpp"
#include "mindnet/plugins/core/enums/ExecutionLayer.hpp"
#include "mindnet/util/Utils.hpp"

// ***** MACROS : END *****

namespace mindnet::plugins::core::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def ERROR_DEFINITION =
        def(COLSERROR::MODEL_NAME, CORE_PLUGIN_NAME)
            .set_rest_operations("rul")
            .set_group("Core", 600)
            .set_columns({
                /* identity */
                coldef(COLSERROR::HUMAN_IDENTIFICATION, MANDATORY | READONLY)
                    .set_description("Public error identifier shown to the user"),
                coldef(COLSERROR::UUID, MANDATORY | READONLY)
                    .set_description("Internal unique error identifier"),

                /* classification */
                coldef(COLSERROR::SEVERITY, MANDATORY | READONLY)
                    .set_enum_definition(
                        mindnet::plugins::core::enums::error_severity_to_enum_definition())
                    .set_description("Severity of the error"),
                coldef(COLSERROR::HTTP_STATUS, INTEGER | READONLY)
                    .set_description("HTTP status associated with the error"),

                /* origin & layer */
                coldef(COLSERROR::ORIGIN, READONLY)
                    .set_enum_definition(
                        mindnet::plugins::core::enums::error_origin_to_enum_definition())
                    .set_description("Execution origin that triggered the error"),
                coldef(COLSERROR::LAYER, READONLY)
                    .set_enum_definition(
                        mindnet::plugins::core::enums::execution_layer_to_enum_definition())
                    .set_description("Build-time layer (CMake target) where the error occurred"),

                /* system location */
                coldef(COLSERROR::PLUGIN, READONLY)
                    .set_description("Plugin in which the error occurred"),
                coldef(COLSERROR::OPERATION, READONLY)
                    .set_enum_definition(enums::crudl_to_enum_definition())
                    .set_description("High-level operation being executed"),

                /* code location */
                coldef(COLSERROR::CPP_NAMESPACE, READONLY)
                    .set_description("C++ namespace where the error occurred"),
                coldef(COLSERROR::CPP_CLASS, READONLY)
                    .set_description("C++ class where the error occurred"),
                coldef(COLSERROR::CPP_SYMBOL, READONLY)
                    .set_description("C++ function or method where the error occurred"),

                /* technical context */
                coldef(COLSERROR::SQL_QUERY, READONLY)
                    .set_description("SQL query associated with the error, if any"),

                /* request / user context */
                coldef(COLSERROR::USER_ID, INTEGER | READONLY | FOREIGN_KEY)
                    .set_description("User associated with the error, if any"),
                coldef(COLSERROR::REQUEST_IDENTIFICATION, READONLY)
                    .set_description("Request correlation identifier"),

                /* payload */
                coldef(COLSERROR::MESSAGE, MANDATORY | READONLY)
                    .set_description("Human-readable error message"),
                coldef(COLSERROR::EXCEPTION_TYPE, READONLY)
                    .set_description("C++ exception type"),
                   coldef(COLSERROR::EXCEPTION_MESSAGE, READONLY),

                /* lifecycle */
                coldef(COLSERROR::HANDLED, BOOL).set_default_value(false)
                    .set_description("Error handling state"),

                /* versioning */
                coldef(COLSERROR::MINDNET_VERSION, READONLY)
                    .set_description("Mind Net version in which the error occurred"),
            });

    struct ModelERROR : mindnet::model::BaseModel
    {
        std::string human_identification;
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
        std::string request_identification;

        std::string message;
        std::string exception_type;
        std::string exception_message;

        bool handled{false};

        std::string mindnet_version;

        static constexpr auto fields = std::make_tuple(
            &ModelERROR::human_identification,
            &ModelERROR::uuid,

            &ModelERROR::severity,
            &ModelERROR::http_status,

            &ModelERROR::origin,
            &ModelERROR::layer,

            &ModelERROR::plugin,
            &ModelERROR::operation,

            &ModelERROR::cpp_namespace,
            &ModelERROR::cpp_class,
            &ModelERROR::cpp_symbol,

            &ModelERROR::sql_query,

            &ModelERROR::user_id,
            &ModelERROR::request_identification,

            &ModelERROR::message,
            &ModelERROR::exception_type,
            &ModelERROR::exception_message,

            &ModelERROR::handled,

            &ModelERROR::mindnet_version
        );

        std::string create_message_for_user() const
        {
            auto status_msg = util::Utils::http_status_to_text(http_status);
            //return status_msg + ". ";
            return "Error ID: " + human_identification;
        }
        create_model_h_methods(ModelERROR, MODELERROR)

        bool operator==(const ModelERROR& other) const
        {
            return id == other.id &&
                   created_at == other.created_at &&
                   updated_at == other.updated_at &&
                   human_identification == other.human_identification &&
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
                   request_identification == other.request_identification &&
                   message == other.message &&
                   exception_type == other.exception_type &&
                   exception_message == other.exception_message &&
                   handled == other.handled &&
                   mindnet_version == other.mindnet_version;
        }
    };
using OptionalError = std::optional<plugins::core::models::Error>;
}

#undef ModelERROR
#undef MODELERROR
#undef COLSERROR
