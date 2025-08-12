///////////////////////////////////////////////////////////////////////////////////////////////
// note-box : Note management tool.
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
#ifndef LITERATURESOURCEREPOSITORY_H
#define LITERATURESOURCEREPOSITORY_H


#include "NoteBox/Entity/LiteratureSource.h"
#include <vector>


namespace NoteBox::Persistence::Api {

    /**
     *
    * @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
     */

    class LiteratureSourceRepository {

    public:
        virtual ~LiteratureSourceRepository() = default;

        virtual void create(const Entity::LiteratureSource& literature_source) = 0;
        virtual Entity::LiteratureSource read(const ushort id) = 0;
        virtual std::vector<Entity::LiteratureSource> list(std::string& title_like) = 0;
        virtual void remove(const Entity::LiteratureSource& literature_source) = 0;
        virtual void update(Entity::LiteratureSource& literature_source) = 0;

    };

}

#endif // LITERATURESOURCEREPOSITORY_H
