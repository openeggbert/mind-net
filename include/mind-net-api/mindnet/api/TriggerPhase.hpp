//
// Created by robertvokac on 9/8/25.
//
#ifndef TRIGGERPHASE_H
#define TRIGGERPHASE_H

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

#endif // TRIGGERPHASE_H
