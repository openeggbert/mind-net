/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "mindnet/util/Utils.hpp"

#include <complex>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <random>
#include <stdexcept>
#include <regex>
#include <openssl/sha.h>

#include "mindnet/essential/Global.hpp"


namespace mindnet::util
{
    using namespace mindnet::essential;

    long long Utils::current_unix_timestamp_seconds()
    {
        return static_cast<long long>(std::time(nullptr));
    }

    long long Utils::current_unix_timestamp_ms()
    {
        using namespace std::chrono;
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }

    std::string Utils::unixtime_to_string(unixtime unixTimestamp)
    {
        if (unixTimestamp == 0)
        {
            return "none";
        }

        // split into seconds and milliseconds
        std::time_t seconds = static_cast<std::time_t>(unixTimestamp / 1000);
        int milliseconds = static_cast<int>(unixTimestamp % 1000);

        std::tm* timeInfo = std::localtime(&seconds);

        // basic format: YYYY-MM-DD HH:MM:SS.mmm
        char buffer[32];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);

        // add milliseconds
        char result[40];
        std::snprintf(result, sizeof(result), "%s.%03d", buffer, milliseconds);

        return {result};
    }

    std::string Utils::unixtime_to_short_string(unixtime unixTimestampMs)
    {
        using namespace std::chrono;

        auto tp = system_clock::time_point(milliseconds(unixTimestampMs));
        std::time_t t = system_clock::to_time_t(tp);
        std::tm tm = *std::localtime(&t);

        return std::format("{:04}{:02}{:02}_{:02}{:02}{:02}",
                           tm.tm_year + 1900,
                           tm.tm_mon + 1,
                           tm.tm_mday,
                           tm.tm_hour,
                           tm.tm_min,
                           tm.tm_sec);
    }

    string Utils::current_unixtime_to_string()
    {
        return Utils::unixtime_to_string(Utils::current_unix_timestamp_ms());
    }

    // Convert single letter 'a'-'z' to number 0-25
    int Utils::letterToDecimal(char letter)
    {
        if (letter < 'a' || letter > 'z')
        {
            throw std::invalid_argument("letterToDecimal: input must be a-z");
        }
        return letter - 'a';
    }

    // Convert number 0-25 to single letter 'a'-'z'
    char Utils::decimal_to_letter(int number)
    {
        if (number < 0 || number > 25)
        {
            throw std::out_of_range("decimalToLetter: input must be 0-25");
        }
        return static_cast<char>('a' + number);
    }

    // Convert base-26 string (a-z) to decimal integer
    // "a" -> 0, "b" -> 1, ..., "z" -> 25, "aa" -> 26, etc.
    int Utils::base26_to_decimal(const std::string& text)
    {
        if (text.empty())
            throw std::invalid_argument("Empty string not allowed");

        for (char c : text)
        {
            if (c < 'a' || c > 'z')
                throw std::invalid_argument("Only lowercase a-z allowed");
        }

        int len = static_cast<int>(text.size());
        int value = 0;

        // Step 1: Add all strings with fewer letters
        int shorter_count = 26;
        for (int i = 1; i < len; ++i)
        {
            value += shorter_count;
            shorter_count *= 26;
        }

        // Step 2: Calculate index within same-length strings
        int offset = 0;
        for (char c : text)
        {
            offset = offset * 26 + letterToDecimal(c);
        }

        return value + offset;
    }

    // Convert decimal integer to base-26 string (a=0, b=1, ..., z=25, aa=26, etc.)
    std::string Utils::decimal_to_base26(int number)
    {
        if (number < 0)
            throw std::invalid_argument("Negative numbers not allowed");

        std::string result;
        int length = 1;

        // Find how many letters are needed
        int count = 26;
        int remaining = number;
        while (remaining >= count)
        {
            remaining -= count;
            length++;
            count *= 26;
        }

        // Build string from remaining number
        for (int i = 0; i < length; ++i)
        {
            int power = length - i - 1;
            int idx = remaining / static_cast<int>(std::pow(26, power));
            result.push_back(decimal_to_letter(idx));
            remaining %= static_cast<int>(std::pow(26, power));
        }

        return result;
    }

    std::vector<std::string> Utils::split_with_quotes(const std::string& input)
    {
        std::vector<std::string> result;
        std::string current;
        bool in_quotes = false;

        for (size_t i = 0; i < input.size(); ++i)
        {
            char c = input[i];

            if (c == '"')
            {
                in_quotes = !in_quotes;
            }
            else if (c == '\\' && i + 1 < input.size() && input[i + 1] == '"')
            {
                // Escaped quote
                current += '"';
                ++i; // Skip next char
            }
            else if (std::isspace(static_cast<unsigned char>(c)) && !in_quotes)
            {
                if (!current.empty())
                {
                    result.push_back(current);
                    current.clear();
                }
            }
            else
            {
                current += c;
            }
        }

        if (!current.empty())
        {
            result.push_back(current);
        }

        return result;
    }

    std::set<string> Utils::split_string_by_commas(const string& string_, std::set<std::string>& result)
    {
        if (!string_.empty())
        {
            std::stringstream ss(string_);
            std::string field_entry;

            while (std::getline(ss, field_entry, ','))
            {
                result.insert(field_entry);
            }
        }
        return result;
    }
    std::vector<string> Utils::split_with_commas(const string& string_)
    {
        std::vector<std::string> result;
        std::stringstream ss(string_);
        std::string item;

        while (std::getline(ss, item, ',')) {
            result.push_back(item);
        }

        return result;
    }

    std::string Utils::compute_sha256(const std::string& text)
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(text.c_str()), text.size(), hash);

        std::ostringstream os;
        for (unsigned char i : hash)
            os << std::hex << std::setw(2) << std::setfill('0') << (int)i;

        return os.str();
    }

    std::string Utils::generate_secret_key(
        size_t length,
        bool numbers,
        bool lower_case,
        bool upper_case,
        bool symbols)
    {
        std::string charset;

        if (numbers)
            charset += "0123456789";
        if (upper_case)
            charset += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        if (lower_case)
            charset += "abcdefghijklmnopqrstuvwxyz";
        if (symbols)
            charset += "!@#$%^&*()-_=+[]{}<>?/|";

        if (charset.empty())
        {
            throw std::invalid_argument("At least one character set must be enabled.");
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, static_cast<int>(charset.size()) - 1);

        std::string key;
        key.reserve(length);
        for (size_t i = 0; i < length; i++)
        {
            key.push_back(charset[dist(gen)]);
        }

        return key;
    }
}
