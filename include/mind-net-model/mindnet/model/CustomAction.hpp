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
#ifndef CUSTOMACTION_H
#define CUSTOMACTION_H

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
#endif // CUSTOMACTION_H
