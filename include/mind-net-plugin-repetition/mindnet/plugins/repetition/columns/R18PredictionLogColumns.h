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

/**
 *
 * @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */
#ifndef R18PREDICTIONLOGCOLUMNS_H
#define R18PREDICTIONLOGCOLUMNS_H

#include "mindnet/model/BaseColumns.h"
#include "mindnet/essential/Helper.h"
#include "mindnet/model/ColumnType.h"

namespace mindnet::plugins::repetition::columns
{
    struct R18PredictionLogColumns : model::BaseColumns
    {
        R18PredictionLogColumns() = delete;

        R18PredictionLogColumns(const R18PredictionLogColumns&) = delete;
        R18PredictionLogColumns& operator=(const R18PredictionLogColumns&) = delete;

        static constexpr const char* MODEL_NAME = "r18_prediction_log";

        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* NOTE_ID = "note_id";
        static constexpr const char* PREDICTED_R_TIMES_100 = "predicted_R_times_100";
        static constexpr const char* ACTUAL_GRADE = "actual_grade";
        static constexpr const char* WAS_CORRECT = "was_correct";
    };
}
#endif // R18PREDICTIONLOGCOLUMNS_H
