//
// Created by robertvokac on 9/7/25.
//

#include <string>
#include <vector>

#include "Plugin.h"
#include "IValidator.h"
#include "jwt-cpp/jwt.h"

namespace mindnet::api
{
    class PluginFactory
    {
    public:
        virtual ~PluginFactory() = default;

        virtual PluginPtr create() const = 0;
    };
}
