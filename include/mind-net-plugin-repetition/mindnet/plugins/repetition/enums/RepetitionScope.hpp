///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation, either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program.
// If not, see <https://www.gnu.org/licenses/> or write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef REPETITION_SCOPE_H
#define REPETITION_SCOPE_H

#include <string>
#include "mindnet/model/EnumDefinition.hpp"
#include "mindnet/essential/EnumUtils.hpp"

namespace mindnet::plugins::repetition::enums
{
    /**
     *
     * @author robertvokac
     */
#define REPETITION_SCOPE_LIST(X, ENUM_NAME) \
X(DueOnly, 0, ENUM_NAME)                \
X(NewOnly, 1, ENUM_NAME)                \
X(DueAndNew, 2, ENUM_NAME)               \
X(All, 3, ENUM_NAME)
    DECLARE_ENUM(RepetitionScope, repetition_scope, REPETITION_SCOPE_LIST)
} // namespace mindnet::plugins::repetition::enums
#endif
