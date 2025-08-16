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
#ifndef COLLECTION_H
#define COLLECTION_H

#include <ostream>
#include <string>
#include "MiniWiki/Helper.h"

namespace miniwiki::Entity {
    using std::string;

    struct Collection {
        int id;
        int parent_collection_id;
        string name;
        string description;
        int order_index;

        Collection(int id_, int parent_collection_id_, string name_, string description_, int order_index_)
            : id(id_),
              parent_collection_id(parent_collection_id_),
              name(std::move(name_)),
              description(std::move(description_)),
              order_index(order_index_) {
        }

        friend std::ostream &operator<<(std::ostream &os, const Collection &collection) {
            os << "Collection{id: " << collection.id
                    << ", parent_collection_id: " << collection.parent_collection_id
                    << ", name: " << collection.name
                    << ", description: " << collection.description
                    << ", order_index: " << collection.order_index
                    << "}";
            return os;
        }

        bool operator==(const Collection &other) const {
            return this->id == other.id &&
                   this->parent_collection_id == other.parent_collection_id &&
                   this->name == other.name &&
                   this->description == other.description &&
                   this->order_index == other.order_index;
        }
    };
}

#endif // COLLECTION_H
