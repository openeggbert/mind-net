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
#ifndef SESSIONREPOSITORY_H
#define SESSIONREPOSITORY_H


#include "NoteBox/Entity/Session.h"


namespace NoteBox::Persistence::Api
{
    /**
     *
    * @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
     */

    class SessionRepository
    {
    public:
        virtual ~SessionRepository() = default;

        virtual void create(const Entity::Session& session) = 0;
        virtual Entity::Session get() = 0;
        virtual void update(const Entity::Session& session) = 0;
    };
}
#endif // SESSIONREPOSITORY_H
