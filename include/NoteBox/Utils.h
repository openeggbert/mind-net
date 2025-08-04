//
// Created by robertvokac on 7/31/25.
//

#ifndef UTILS_H
#define UTILS_H

namespace NoteBox {

class Utils {
private:
    Utils() = delete;
    Utils(const Utils&) = delete;
    Utils& operator=(const Utils&) = delete;
public:
    static long long currentTimestamp();
    static void clearScreen();
};

} // NoteBox

#endif //UTILS_H
