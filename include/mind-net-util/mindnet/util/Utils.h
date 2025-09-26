//
// Created by robertvokac on 7/31/25.
//

#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>

#include "mindnet/essential/Helper.h"

namespace mindnet::util
{
    /**
 * @brief Result structure for SuperMemo 2 (SM-2) algorithm calculations
 *
 * This structure holds the calculated values for next review scheduling:
 * - new_interval: Number of days until next review
 * - new_ef: New easiness factor (minimum 1.3)
 * - new_repetition: Number of successful reviews + 1 (reset to 0 on quality < 3)
 *
 * The values are used to update question_sm2_state:
 * question_sm2_state.repetitions = result.new_repetition;
 * question_sm2_state.interval = result.new_interval;
 * question_sm2_state.ef_times_100 = result.new_ef * 100.0;
 * question_sm2_state.next_review = now + result.new_interval * 86400; // in seconds
 * question_sm2_state.last_review = now;
 * question_sm2_state.last_quality = quality;
 */

    struct SM2Result
    {
        int new_interval;
        double new_ef;
        int new_repetition;
    };

    class Utils
    {
    private:
        Utils() = delete;
        Utils(const Utils&) = delete;
        Utils& operator=(const Utils&) = delete;

    public:
        static SM2Result calculate_sm2(int quality, int current_repetition, int current_interval, double current_ef);
        static long long currentTimestamp();
        static long long currentUnixTimestamp();
        static string unixToFormattedString(ll unixTimestamp);
        static string print_current_timestamp();
        // static ll formattedStringToUnix(const std::string& formattedTime);

        // Convert single letter 'a'-'z' to number 0-25
        static int letterToDecimal(char letter);

        // Convert number 0-25 to single letter 'a'-'z'
        static char decimalToLetter(int number);

        // Convert base-26 string (a-z) to decimal integer
        // "a" -> 0, "b" -> 1, ..., "z" -> 25, "aa" -> 26, etc.
        static int base26ToDecimal(const std::string& text);

        // Convert decimal integer to base-26 string (a-z)
        static std::string decimalToBase26(int number);

        static std::vector<std::string> split_with_quotes(const std::string& input);
        static std::set<string> split_string_by_commas(const string& string_,
                                                       std::set<std::string>& result);
        static std::string hash_sha_256(const std::string& text);


        template <typename K, typename V>
        struct MapDiff
        {
            std::map<K, V> added;
            std::map<K, V> removed;
            std::map<K, std::pair<V, V>> changed; // old, new
        };

        template <typename K, typename V>
        static MapDiff<K, V> diff_maps(const std::map<K, V>& oldMap, const std::map<K, V>& newMap)
        {
            MapDiff<K, V> diff;

            auto itOld = oldMap.begin();
            auto itNew = newMap.begin();

            while (itOld != oldMap.end() || itNew != newMap.end())
            {
                if (itOld == oldMap.end())
                {
                    // everything remaining in newMap is added
                    diff.added[itNew->first] = itNew->second;
                    ++itNew;
                }
                else if (itNew == newMap.end())
                {
                    // everything remaining in oldMap is removed
                    diff.removed[itOld->first] = itOld->second;
                    ++itOld;
                }
                else if (itOld->first < itNew->first)
                {
                    // key exists only in oldMap
                    diff.removed[itOld->first] = itOld->second;
                    ++itOld;
                }
                else if (itNew->first < itOld->first)
                {
                    // key exists only in newMap
                    diff.added[itNew->first] = itNew->second;
                    ++itNew;
                }
                else
                {
                    // key exists in both maps
                    if (itOld->second != itNew->second)
                    {
                        diff.changed[itOld->first] = {itOld->second, itNew->second};
                    }
                    ++itOld;
                    ++itNew;
                }
            }

            return diff;
        }

        template <typename K, typename V>
        static std::string diff_maps_to_string(MapDiff<K, V> diff)
        {
            std::ostringstream ss;
            ss << "Added:\n";
            for (auto& [k,v] : diff.added)
                ss << "  " << k << " = " << v << "\n";

            ss << "Removed:\n";
            for (auto& [k,v] : diff.removed)
                ss << "  " << k << " = " << v << "\n";

            ss << "Changed:\n";
            for (auto& [k,pair] : diff.changed)
                ss << "  " << k << ": " << pair.first << " -> " << pair.second << "\n";
            return ss.str();
        }
    };

    enum EscapeColor
    {
        Black = 0, Red = 1, Green = 2, Yellow = 3, Blue = 4, Magenta = 5, Cyan = 6, White = 7,
        BrightBlack = 8, BrightRed = 9, BrightGreen = 10, BrightYellow = 11, BrightBlue = 12, BrightMagenta = 13,
        BrightCyan = 14, BrightWhite = 15,
    };

    struct TextColour
    {
        int foreground = -1;
        int background = -1;
    };

    inline void set_text_colour(TextColour colour)
    {
        if (colour.foreground >= 0) printf("\033[38;5;%dm", colour.foreground);
        if (colour.background >= 0) printf("\033[48;5;%dm", colour.background);
    }

    inline void set_text_colour(EscapeColor foreground, EscapeColor background)
    {
        set_text_colour({foreground, background});
    }

    inline void set_text_colour(EscapeColor foreground)
    {
        set_text_colour({foreground, -1});
    }
}

#endif //UTILS_H
