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
    static void log(const char* message);
    static void log(const std::string& message);
    static void trace(const char* message);
    static bool ask_yes_no(const std::string& question);
    static std::string editTextInEditor(const std::string& inputText, const std::string& editorPath);
};

} // NoteBox

#endif //UTILS_H
