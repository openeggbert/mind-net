//
// Created by robertvokac on 9/8/25.
//
#ifndef CONSOLECOLOR_H
#define CONSOLECOLOR_H


#include <string>

namespace mindnet::core
{

    enum class ConsoleColor
    {
        UNKNOWN = -1,
        BLACK = 30,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        PURPLE = 35,
        CYAN = 36,
        WHITE = 37
    };
}
#endif // CONSOLECOLOR_H
