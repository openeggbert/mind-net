/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once


#include <string>
#include "mindnet/model/EnumDefinition.hpp"


namespace mindnet::plugins::slipbox::enums
{
    /**
     *
     * @author robertvokac
     */
    enum class AlertStatus
    {
        PENDING = 0,
        TRIGGERED = 1,
        SNOOZED = 2,
        DISMISSED = 3,
        FAILED = 4
    };

    inline std::string alert_status_to_string(AlertStatus status)
    {
        switch (status)
        {
        case AlertStatus::PENDING:
            return "PENDING";
        case AlertStatus::TRIGGERED:
            return "TRIGGERED";
        case AlertStatus::SNOOZED:
            return "SNOOZED";
        case AlertStatus::DISMISSED:
            return "DISMISSED";
        case AlertStatus::FAILED:
            return "FAILED";
        default:
            return "Unknown";
        }
    }

    inline std::string alert_status_to_string(int status)
    {
        return alert_status_to_string(static_cast<AlertStatus>(status));
    }

    inline model::EnumDefinition alert_status_to_enum_definition()
    {
        return model::EnumDefinition{
            alert_status_to_string, 5, 0, 1, 2, 3, 4
        };
    }
} // namespace mindnet::plugins::slipbox::enums
