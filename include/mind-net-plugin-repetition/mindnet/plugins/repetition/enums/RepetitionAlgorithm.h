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
#ifndef REPETITIONALGORITHM_H
#define REPETITIONALGORITHM_H


#include <string>
#include "mindnet/model/EnumDefinition.h"

namespace mindnet::plugins::repetition::enums
{
    /**
     *
     * @author robertvokac
     */
    enum class RepetitionAlgorithm
    {
        Repetition0 = 0,
        Repetition2 = 2,
        Repetition4 = 4,
        Repetition18 = 18,
    };

    inline std::string repetition_algorithm_to_string(RepetitionAlgorithm algorithm)
    {
        switch (algorithm)
        {
        case RepetitionAlgorithm::Repetition0:
            return "Repetition0";
        case RepetitionAlgorithm::Repetition2:
            return "Repetition2";
        case RepetitionAlgorithm::Repetition4:
            return "Repetition4";
        case RepetitionAlgorithm::Repetition18:
            return "Repetition18";
        default:
            return "Unknown";
        }
    }

    inline std::string repetition_algorithm_to_string(int algorithm)
    {
        return repetition_algorithm_to_string(static_cast<RepetitionAlgorithm>(algorithm));
    }

    inline model::EnumDefinition repetition_algorithm_to_enum_definition()
    {
        return model::EnumDefinition{
            repetition_algorithm_to_string, 4, 0, 2, 4, 18
        };
    }
} // namespace mindnet::plugins::repetition::enums
#endif // REPETITIONALGORITHM_H
