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
#ifndef TERMREPOSITORY_H
#define TERMREPOSITORY_H

#include <optional>

#include "MiniWiki/Entity/Term.h"
#include <string>
#include <vector>

namespace MiniWiki::Persistence::Api
{
    /**
     *
    * @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
     */

    class TermRepository
    {
    public:
        virtual ~TermRepository() = default;

        virtual void create(const Entity::Term& term) = 0;
        virtual Entity::Term read(const std::string& id) = 0;
        virtual void update(const Entity::Term& term) = 0;
        virtual str id_for_name_and_category(str& name, str& category) = 0;
        virtual void remove(const std::string& id) = 0;
        virtual std::vector<Entity::Term> list(std::string& category, size_t page_number, size_t pageSize) = 0;
        virtual std::vector<str> list_categories() = 0;
    };
}

#endif // TERMREPOSITORY_H
