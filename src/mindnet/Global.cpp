//
// Created by robertvokac on 7/31/25.
//

#include "mindnet/Global.h"

namespace mindnet
{
    ll start_time = 0l;

    Logger fatal(FATAL);
    Logger err(ERROR);
    Logger warn(WARN);
    Logger info(INFO);
    Logger debug(DEBUG);
    Logger trace(TRACE);
    Logger experiment(EXPERIMENT);
    //
    Logger& log = info;
}
