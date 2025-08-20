//
// Created by robertvokac on 8/8/25.
//
#ifndef REDCOUT_H
#define REDCOUT_H
#include <iostream>
#include <sstream>

namespace mindnet
{
    class RedCout
    {
        std::ostringstream buffer;

    public:
        template <typename T>
        RedCout& operator<<(const T& value)
        {
            buffer << value;
            return *this;
        }

        RedCout& operator<<(std::ostream& (*manip)(std::ostream&))    {
            std::cout << "\033[31m" << buffer.str() << "\033[0m";
            manip(std::cout); // např. std::endl
            buffer.str(""); // vyčistit buffer
            buffer.clear();
            return *this;
        }
    };
}
#endif // REDCOUT_H
