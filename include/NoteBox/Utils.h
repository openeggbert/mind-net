//
// Created by robertvokac on 7/31/25.
//

#ifndef UTILS_H
#define UTILS_H
#include <string>

namespace NoteBox {

class Utils {
private:
    Utils() = delete;
    Utils(const Utils&) = delete;
    Utils& operator=(const Utils&) = delete;
public:
    static long long currentTimestamp();
    static void clearScreen();
    static std::string createJdbcUrl(const std::string& directoryWhereSqliteFileIs);
};

} // NoteBox

#endif //UTILS_H
