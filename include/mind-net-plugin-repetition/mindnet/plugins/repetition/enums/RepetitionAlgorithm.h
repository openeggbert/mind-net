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
#ifndef REPETITIONALGORITHM_H
#define REPETITIONALGORITHM_H

#include <string>
#include "mindnet/model/EnumDefinition.h"
#include "mindnet/essential/EnumUtils.h"

namespace mindnet::plugins::repetition::enums
{
    /**
     *
     * @author robertvokac
     */
#define REPETITION_ALGORITHM_LIST(X, ENUM_NAME) \
X(Repetition0, 0, ENUM_NAME)               \
X(Repetition2, 2, ENUM_NAME)               \
X(Repetition4, 4, ENUM_NAME)               \
X(Repetition18, 18, ENUM_NAME)

    DECLARE_ENUM(RepetitionAlgorithm, repetition_algorithm, REPETITION_ALGORITHM_LIST)

} // namespace mindnet::plugins::repetition::enums
#endif // REPETITIONALGORITHM_H
