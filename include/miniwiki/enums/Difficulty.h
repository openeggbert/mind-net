///////////////////////////////////////////////////////////////////////////////////////////////
// mini-wiki : Lightweight wiki inspired by MediaWiki.
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
#ifndef DIFFICULTY_H
#define DIFFICULTY_H

#include <string>

namespace miniwiki::Enum {
    /**
     *
     * @author robertvokac
     */
    enum class Difficulty {
        EASY = 1, MEDIUM = 2, HARD = 3, EXPERT = 4
    };
    inline std::string difficulty_to_string(Difficulty difficulty) {
        switch (difficulty)
        {
            case Difficulty::EASY:
                return "Easy";
            case Difficulty::MEDIUM:
                return "Medium";
            case Difficulty::HARD:
                return "Hard";
            case Difficulty::EXPERT:
                return "Expert";
            default:
                return "Unknown";
        }
    }
    inline std::string difficulty_to_string(int difficulty)
    {
        return difficulty_to_string(static_cast<Difficulty>(difficulty));
    }



} // namespace miniwiki::Enums
#endif
