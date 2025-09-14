//
// Created by robertvokac on 9/13/25.
//
#ifndef ZETTELKASTENPLUGINFACTORY_H
#define ZETTELKASTENPLUGINFACTORY_H

#include "mindnet/api/PluginFactory.h"

namespace mindnet::plugins::zettelkasten
{
    class ZettelkastenPluginFactory : public api::PluginFactory
    {
    public:
        api::PluginPtr create() const override;
    };
}

#endif // ZETTELKASTENPLUGINFACTORY_H
