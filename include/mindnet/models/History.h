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
#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include "mindnet/models/misc/BaseModel.h"

// ***** DEFINE SECTION : START *****
#define Model History
#define MODEL HISTORY
#define model_columns_h_file "columns/HistoryColumns.h"
// ***** DEFINE SECTION : END *****
#include model_columns_h_file
start_models_namespace
start_model_definition(Model, MODEL)
// ***** DEFINE MODEL DEFINITION : START *****
auto_inc
allowed_rest_operations({crudl::READ, crudl::LIST})
start_columns_definition
//
defcol(USER_ID) mandatory_ foreign_key_("user") enddefcol
defcol(IP_ADDRESS) enddefcol
defcol(TABLE_NAME) mandatory_ enddefcol
defcol(RECORD_ID) mandatory_ enddefcol
defcol(OPERATION)  mandatory_ enum_(enums::crudl_to_enum_definition()) enddefcol
defcol(DATA_JSON) mandatory_ enddefcol
defcol(REASON) enddefcol
//
end_columns_definition
end_model_definition
// ***** DEFINE MODEL DEFINITION : END *****

start_model_struct(Model)

// ***** DEFINE FIELDS : START *****
str table_name;
int record_id{};
enums::Crudl operation{};
str data_json;
str reason;
// ***** DEFINE FIELDS : END *****

create_model_h_methods(Model, MODEL)

// ***** Implement methods operator== : START *****
bool operator==(const Model & other) const
{
    return id == other.id &&
        created_at == other.created_at &&
        updated_at == other.updated_at &&
        table_name == other.table_name &&
        record_id == other.record_id &&
        operation == other.operation &&
        data_json == other.data_json &&
        reason == other.reason;
}
// ***** Implement methods operator== : END *****

end_model_struct
end_models_namespace
#undef Model
#undef MODEL
#endif // HISTORY_H
