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
#ifndef IDEA_H
#define IDEA_H

#include <ostream>
#include <string>

#include "BaseEntity.h"
#include "NoteBox/Helper.h"
#include "NoteBox/Utils.h"
#include "NoteBox/Persistence/Impl/Sqlite/Tables/IdeaTable.h"

namespace NoteBox::Entity
{
    using std::string;

    struct Idea : BaseEntity
    {
        int id;
        string text;
        string category;
        unixtime created_at;
        unixtime due_date;

        [[nodiscard]] str get_entity_name() const override
        {
            return Persistence::Impl::Sqlite::Tables::IdeaTable::TABLE_NAME;
        };

        entity_columns get_entity_columns() const override
        {
            return Persistence::Impl::Sqlite::Tables::IdeaTable::get_column_names();
        };
        entity_fields get_entity_fields() const override;
        bool should_be_id_auto_incremented() const override {return true;}
        friend std::ostream& operator<<(std::ostream& os, const Idea& idea)
        {
            os << idea.to_json();


            return os;
        }

        bool operator==(const Idea& other) const
        {
            return this->equals(other);

        }
    };
}
#endif // IDEA_H
