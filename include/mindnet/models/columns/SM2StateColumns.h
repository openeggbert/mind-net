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

/**
 *
* @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */
#ifndef SM2RESULTCOLUMNS_H
#define SM2RESULTCOLUMNS_H


#include "BaseColumns.h"
#include "mindnet/Helper.h"
#include "mindnet/enums/ColumnType.h"


namespace mindnet::models::columns
{
    struct SM2StateColumns : BaseColumns
    {
        SM2StateColumns() = delete;

        SM2StateColumns(const SM2StateColumns&) = delete;
        SM2StateColumns& operator=(const SM2StateColumns&) = delete;

        static constexpr const char* MODEL_NAME = "sm2_result";

        static constexpr const char* INTERVAL = "interval";
        static constexpr const char* EF_TIMES_100 = "ef_times_100";
        static constexpr const char* REPETITION = "repetition";
    };
}
#endif // SM2RESULTCOLUMNS_H
