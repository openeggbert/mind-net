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



#include <map>

#include "ostream"
#include <string>
#include <utility>

#include "mindnet/essential/Configuration.hpp"
#include "mindnet/essential/DatabaseType.hpp"


namespace mindnet::model
{
    using std::string;
    typedef std::map<std::string, std::string> string_map;

    /**
     * Represents the definition of a database model including its name,
     * auto-increment status, columns and allowed CRUD operations.
     */
    struct CustomAction
    {
    public:
        string action;

        string label;
        std::string crudl;
        string model_name;
        string_map params;

    public:
        CustomAction(
            mindnet::essential::Crudl crudl_,
            std::string model_name_,
            std::string label_,
            const string_map& params_ = {}
        ) :
            crudl(essential::crudl_to_string(static_cast<int>(crudl_))),
            model_name(std::move(model_name_)),
            label(std::move(label_)),
            params(params_)
        {
            std::string action_ = label;
            for (auto& c : action_) c = std::tolower(c);
            std::replace(action_.begin(), action_.end(), ' ', '_');
            action = action_;
        }
    };
}
