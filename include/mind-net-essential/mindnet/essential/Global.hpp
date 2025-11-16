//
// Created by robertvokac on 7/31/25.
//

#pragma once


#include "Helper.hpp"
#include "Logger.hpp"

#define using_loggers()\
using mindnet::essential::fatal;\
using mindnet::essential::err;\
using mindnet::essential::warn;\
using mindnet::essential::info;\
using mindnet::essential::debug;\
using mindnet::essential::trace;\
using mindnet::essential::experiment;\
using mindnet::essential::commit;

namespace mindnet::essential
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

