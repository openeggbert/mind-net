///////////////////////////////////////////////////////////////////////////////////////////////
// mini-wiki : Note management tool.
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
#ifndef IDEAREPOSITORY_H
#define IDEAREPOSITORY_H

#include <optional>
#include "miniwiki/models/Idea.h"
#include <string>
#include <vector>

namespace MiniWiki::Persistence::Api
{
    /**
     *
    * @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
     */

    class IdeaRepository
    {
    public:
        virtual ~IdeaRepository() = default;

        virtual void create(const Entity::Idea& idea) = 0;
        virtual Entity::Idea read(int id) = 0;
        virtual void update(const Entity::Idea& idea) = 0;
        virtual void remove(int id) = 0;
        virtual std::vector<Entity::Idea> list(str& category, size_t page_number, size_t pageSize) = 0;
    };
}

#endif // IDEAREPOSITORY_H
