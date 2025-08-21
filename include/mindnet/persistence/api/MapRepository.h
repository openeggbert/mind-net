///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : Mind map software.
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
#ifndef MAPREPOSITORY_H
#define MAPREPOSITORY_H


#include "mindnet/models/Map.h"
#include <vector>

#include "IRepository.h"
#include "mindnet/persistence/impl/sqlite/SqliteFileName.h"
#include "SQLiteCpp/Database.h"


namespace mindnet::persistence::api {

    /**
     *
    * @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
     */

    class MapRepository : public models::IRepository {

    public:
        MapRepository() = default;
        virtual ~MapRepository() = default;

        virtual int create(const models::BaseModel& content) = 0;
        virtual entity_fields read(int id, models::ModelDefinition& def) = 0;

        virtual models::ModelDefinition& get_model_definition() = 0;


    public:

    private:


        // virtual void update(models::Map& content) = 0;
        // virtual void remove(int id) = 0;
        // virtual std::vector<models::Map> list(size_t page_number, size_t pageSize) = 0;
    };

}

#endif // MAPREPOSITORY_H
