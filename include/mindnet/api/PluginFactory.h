//
// Created by robertvokac on 9/7/25.
//
#ifndef PLUGINFACTORY_H
#define PLUGINFACTORY_H

#define REGISTER_MODEL(model, Model, MODEL)\
plugin->register_model(\
        models::MODEL##_DEFINITION,\
        std::make_shared<validators::Model##Validator>(),\
        impl::sqlite::request_to_entity_fields_##model );

#include "Plugin.h"

namespace mindnet::api
{
    class PluginFactory
    {
    public:
        virtual ~PluginFactory() = default;

        virtual PluginPtr create() const = 0;
    };
}
#endif // PLUGINFACTORY_H
