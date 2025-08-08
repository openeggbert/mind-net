//
// Created by robertvokac on 7/31/25.
//

#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>

#include "Helper.h"

namespace NoteBox
{
    class Utils
    {
    private:
        Utils() = delete;
        Utils(const Utils&) = delete;
        Utils& operator=(const Utils&) = delete;

    public:
        static long long currentTimestamp();
        static long long currentUnixTimestamp();
        static std::string unixToFormattedString(ll unixTimestamp);
        // static ll formattedStringToUnix(const std::string& formattedTime);

        static void clearScreen();
        static void log(const char* message);
        static void log(const std::string& message);
        static void trace(const char* message);
        static bool ask_yes_no(const std::string& question);
        static std::string editTextInEditor(const std::string& inputText, const std::string& editorPath);
        /**
     * Converts a note identifier string into a vector of string components.
     * The note ID is split into alternating sections of digits and letters.
     *
     * @param note_id The note identifier string to be split. This string must start with a digit.
     *                An empty string or one that doesn't start with a digit will result in an exception.
     *
     * @return A vector containing the split components of the note ID in the order of their appearance.
     *         Each component will either be a series of digits or a series of letters.
     *
     * @throws std::runtime_error If the provided note_id is empty or does not start with a digit.
     */
        static std::vector<std::string> note_id_to_vector(const std::string& note_id);
    };
} // NoteBox

#endif //UTILS_H
