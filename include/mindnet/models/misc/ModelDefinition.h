///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : Mind map software.
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
#ifndef MODELDEFINITION_H
#define MODELDEFINITION_H

#include <set>

#include "ostream"
#include <string>

#include "mindnet/Helper.h"
#include "mindnet/enums/Crudl.h"

namespace mindnet::models::misc {
    using std::string;

    struct ModelDefinition {
        str model_name;
        bool auto_increment;
        column_definitions columns;
        std::set<enums::Crudl> allowed_crudl_rest_operations;
    };
}

#endif // MODELDEFINITION_H
