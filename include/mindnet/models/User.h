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
#ifndef USER_H
#define USER_H


#include <string>
#include "mindnet/models/misc/BaseModel.h"

// ***** DEFINE SECTION : START *****
#define Model User
#define MODEL USER
#define model_columns_h_file "columns/UserColumns.h"
// ***** DEFINE SECTION : END *****
#include model_columns_h_file
start_models_namespace
start_model_definition(Model, MODEL)
// ***** DEFINE MODEL DEFINITION : START *****
auto_inc
start_columns_definition
//
defcol(USERNAME) mandatory_ unique_ enddefcol
defcol(PASSWORD_HASH) mandatory_ enddefcol
defcol(DISPLAY_NAME) enddefcol
defcol(ROLE) mandatory_ enum() TODO enddefcol
defcol(PROFILE_TEXT) textarea_ enddefcol
defcol(LAST_LOGIN) datetime_ enddefcol
defcol(EMAIL) unique_ enddefcol
defcol(STATUS) mandatory_ enddefcol





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

#endif // USER_H
