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

#include <functional>
#include <memory>

#include "AccessTokenContext.hpp"
#include "OperationResult.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/essential/Helper.hpp"

// 🔐 Validator as a Security Gate
// Validator does more than just check data — it acts as protection against unauthorized access:
//
// Authentication: Is the user logged in?
// Authorization: Does the user have the right to read/modify the given entity?
// Integrity: Does the operation preserve the system’s logic?
// Consistency: Are key dependencies present (e.g., a parent note)?

namespace mindnet::api
{
    class IPersistence;
}

namespace mindnet::api
{
    class IValidator;
    typedef std::function<std::shared_ptr<IValidator>(const std::string&)> GetValidatorFunc;

    using string = std::string;

    class IValidator
    {
    public:
        virtual ~IValidator() = default;

        typedef std::shared_ptr<IPersistence> DbPtr;

        virtual OperationResult can_create(DbPtr& db, api::AccessTokenContext& token, entity_fields& ef) const = 0;

        virtual OperationResult can_read(DbPtr& db, api::AccessTokenContext& token, identification id) const = 0;

        virtual OperationResult can_update(DbPtr& db, api::AccessTokenContext& token, entity_fields& ef,
                                           entity_fields& old_fields) const = 0;

        virtual OperationResult can_delete(DbPtr& db, api::AccessTokenContext& token, identification id) const = 0;

        virtual OperationResult can_list(DbPtr& db, api::AccessTokenContext& token, string_map& filter) const = 0;

        [[nodiscard]] virtual string get_model_name() const = 0;

        virtual void set_validator_func(GetValidatorFunc func) = 0;
        [[nodiscard]] virtual GetValidatorFunc get_validator_func() const = 0;

        [[nodiscard]] std::shared_ptr<IValidator> get_validator(const string& model_name) const
        {
            auto func = get_validator_func();
            if (!func) return nullptr;
            return func(model_name);
        }
    };
}
