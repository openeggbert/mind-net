//
// Created by robertvokac on 7/31/25.
//

#ifndef GLOBAL_H
#define GLOBAL_H

#include "Helper.h"
#include "Logger.h"

#define using_loggers()\
using mindnet::core::fatal;\
using mindnet::core::err;\
using mindnet::core::warn;\
using mindnet::core::info;\
using mindnet::core::debug;\
using mindnet::core::trace;\
using mindnet::core::experiment;\
using mindnet::core::commit;

namespace mindnet::core
{
    extern ll start_time;
    extern Logger fatal;
    extern Logger err;
    extern Logger warn;
    extern Logger info;
    extern Logger debug;
    extern Logger trace;
    extern Logger experiment;
    //
    extern Logger& log;
}

#endif //GLOBAL_H
