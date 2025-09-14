//
// Created by robertvokac on 9/8/25.
//
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>

#include "model/EnumDefinition.h"

namespace mindnet
{
    enum class Environment
    {
        Production = 0,
        Staging = 1,
        Development = 2
    };

    inline std::string environment_to_string(const Environment& environment)
    {
        switch (environment)
        {
        case Environment::Production:
            return "Production";
        case Environment::Staging:
            return "Staging";
        case Environment::Development:
            return "Development";
        default:
            return "Unknown";
        }
    }

    inline std::string environment_to_string(int environment)
    {
        return environment_to_string(static_cast<Environment>(environment));
    }

    inline Environment string_to_environment(const std::string& env_str)
    {
        if (env_str == "Production") return Environment::Production;
        if (env_str == "Staging") return Environment::Staging;
        if (env_str == "Development") return Environment::Development;
        throw std::runtime_error("Invalid environment: " + env_str);
    }

    inline model::EnumDefinition environment_to_enum_definition()
    {
        return model::EnumDefinition{
            environment_to_string, 3, 0, 1, 2
        };
    }
}
#endif // ENVIRONMENT_H
