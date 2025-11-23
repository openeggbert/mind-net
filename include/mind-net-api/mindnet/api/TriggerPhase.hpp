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

#include <stdexcept>
#include <string>

#include "mindnet/model/EnumDefinition.hpp"


namespace mindnet::api
{
    enum class TriggerPhase
    {
        Before = 0,
        After = 1,
        InsteadOf = 2, // Trigger is executed instead of the operation
        Around = 3 // Like before and after together
    };

    inline std::string trigger_phase_to_string(TriggerPhase trigger_phase)
    {
        switch (trigger_phase)
        {
        case TriggerPhase::Before:
            return "Before";
        case TriggerPhase::After:
            return "After";
        case TriggerPhase::InsteadOf:
            return "InsteadOf";
        case TriggerPhase::Around:
            return "Around";
        default:
            return "Unknown";
        }
    }

    inline std::string trigger_phase_to_string(int trigger_phase)
    {
        return trigger_phase_to_string(static_cast<TriggerPhase>(trigger_phase));
    }

    inline TriggerPhase string_to_trigger_phase(const std::string& phase_str)
    {
        if (phase_str == "Before") return TriggerPhase::Before;
        if (phase_str == "After") return TriggerPhase::After;
        if (phase_str == "InsteadOf") return TriggerPhase::InsteadOf;
        if (phase_str == "Around") return TriggerPhase::Around;
        throw std::runtime_error("Invalid trigger phase: " + phase_str);
    }

    inline model::EnumDefinition trigger_phase_to_enum_definition()
    {
        return model::EnumDefinition{
            trigger_phase_to_string, 4, 0, 1, 2, 3
        };
    }
}

