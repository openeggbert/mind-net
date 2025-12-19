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

#include "mindnet/plugins/core/enums/Crudl.hpp"
#include "mindnet/plugins/core/enums/ErrorSeverity.hpp"
#include "mindnet/plugins/core/enums/ErrorOrigin.hpp"
#include "mindnet/plugins/core/enums/ExecutionLayer.hpp"
#include <array>

#include "models/Error.hpp"

namespace mindnet::plugins::core
{
    using std::string;
    static std::string generate_uuid_v7();
    //MN-ERR-20251219-4F9C7A
    static std::string generate_public_id();

    class ErrorBuilder
    {
    public:
        ErrorBuilder(
            const std::string& message,
            enums::ErrorSeverity s,
            const enums::ErrorOrigin origin,
            const enums::ExecutionLayer layer
            );
        ErrorBuilder& http_status(int status);

        ErrorBuilder& plugin(std::string v);
        ErrorBuilder& operation(essential::Crudl o);

        ErrorBuilder& cpp(string cpp_namespace, string cpp_class, string cpp_symbol);
        ErrorBuilder& sql(std::string sql);

        ErrorBuilder& user(identification id);
        ErrorBuilder& request(std::string id);

        ErrorBuilder& exception(const std::exception& e);

        models::Error build();

    private:
        models::Error model_;
    };

}

