#include "hive/plugins/dictionary/DictionaryUtils.hpp"
//
// Created by robertvokac on 12/13/25.
//
namespace hive::plugins::dictionary
{

    std::string is_tag_or_flag_title_valid(const std::string& title)
    {
        if (title.empty())
            return "tag must not be empty";

        if (title[0] == '-')
            return "tag must not start with '-'";

        char prev = '\0';

        for (char c : title)
        {
            // forbid two consecutive dashes
            if (c == '-' && prev == '-')
                return "tag must not contain consecutive '-' characters";

            // allow: lowercase letters
            if (c >= 'a' && c <= 'z') { prev = c; continue; }

            // allow: digits
            if (c >= '0' && c <= '9') { prev = c; continue; }

            // allow separators / modifiers
            switch (c)
            {
            case '-':
            case '+':
            case '_':
            case '.':
                prev = c;
                continue;
            }

            return std::string("invalid character in tag: '") + c + "'";
        }

        return "";
    }

}