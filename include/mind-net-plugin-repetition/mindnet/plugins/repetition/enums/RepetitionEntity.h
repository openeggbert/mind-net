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
#ifndef REPETITIONENTITY_H
#define REPETITIONENTITY_H

#include <string>
#include "mindnet/model/EnumDefinition.h"
#include "mindnet/essential/EnumUtils.h"

namespace mindnet::plugins::repetition::enums
{
    /**
     * @enum RepetitionEntity
     *
     * Defines the type of entity being used in a repetition session.
     * 
     * - Note (0): Represents a note entity in the system
     * - Question (1): Represents a question entity in the system
     *
     * @author
     *   Robert Vokac
     */
#define REPETITION_ENTITY_LIST(X, ENUM_NAME)     \
    X(Note, 0, ENUM_NAME)                        \
    X(Question, 1, ENUM_NAME)

    DECLARE_ENUM(RepetitionEntity, repetition_entity, REPETITION_ENTITY_LIST)
} // namespace mindnet::plugins::repetition::enums

#endif // REPETITIONENTITY_H
