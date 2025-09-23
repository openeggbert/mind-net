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
#ifndef REPOSITORYHELPER_H
#define REPOSITORYHELPER_H

#include <iostream>
#include <vector>

#include "SqliteFileName.h"
#include "../../util/Utils.h"
#include "../../orm/QueryParams.h"
#include "SQLiteCpp/Database.h"

namespace mindnet::db::sqlite
{
    using std::vector;
    using sqlite::SQLITE_FILE_NAME;

    void set_pragmas(SQLite::Database& db);

    int create_model(const entity_fields& fields, const model::ModelDefinition& definition, string& error);

    entity_fields read_model(model::ModelDefinition& def, int id, string& error);

    bool update_model(int id, model::ModelDefinition& def, entity_fields& fields, string& error);

    bool delete_model(model::ModelDefinition& def, int id, string& error);

    std::vector<entity_fields> list_models(
        model::ModelDefinition& def,
        mindnet::orm::QueryParams& query_params,
        string& error
    );
}

#endif // REPOSITORYHELPER_H
