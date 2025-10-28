//
// Created by robertvokac on 10/23/25.
//

#include "mindnet/plugins/slipbox/triggers/ContentLinkParser.h"
#include <regex>

namespace mindnet::plugins::slipbox::triggers
{
    inline std::string rtrim_url_punct(std::string s)
    {
        auto is_simple_punct = [](char c)
        {
            return c == ',' || c == '.' || c == ';' || c == ':' || c == '"' || c == '\'' || c == '>';
        };
        while (!s.empty() && is_simple_punct(s.back())) s.pop_back();

        // Bracket counting remains unchanged...
        size_t paren_o = 0, paren_c = 0, sq_o = 0, sq_c = 0, cur_o = 0, cur_c = 0;
        for (char c : s)
        {
            paren_o += (c == '(');
            paren_c += (c == ')');
            sq_o += (c == '[');
            sq_c += (c == ']');
            cur_o += (c == '{');
            cur_c += (c == '}');
        }
        auto trim_excess = [&](char close, size_t excess)
        {
            while (excess && !s.empty() && s.back() == close)
            {
                s.pop_back();
                --excess;
            }
        };
        if (paren_c > paren_o) trim_excess(')', paren_c - paren_o);
        if (sq_c > sq_o) trim_excess(']', sq_c - sq_o);
        if (cur_c > cur_o) trim_excess('}', cur_c - cur_o);

        // Optional: trim typographic ellipsis (UTF-8: E2 80 A6)
        if (s.size() >= 3 && static_cast<unsigned char>(s[s.size() - 3]) == 0xE2
            && static_cast<unsigned char>(s[s.size() - 2]) == 0x80
            && static_cast<unsigned char>(s[s.size() - 1]) == 0xA6)
        {
            s.resize(s.size() - 3);
        }
        return s;
    }

    static const std::regex kUrlRe(
        R"((https?|ftp)://[^\s<>"'()\[\]]+)",
        std::regex::icase
    );

    std::vector<Match> find_urls(const std::string& s)
    {
        std::vector<Match> out;
        out.reserve(16);

        for (std::sregex_iterator it(s.begin(), s.end(), kUrlRe), end; it != end; ++it)
        {
            const auto& m = *it;
            std::size_t pos = static_cast<std::size_t>(m.position());
            std::string cleaned = rtrim_url_punct(m.str());
            out.push_back({cleaned, pos, cleaned.size()});
            if (out.size() > 10000) break;
        }
        return out;
    }

    inline std::string wikilink_title(std::string inner)
    {
        if (inner.size() > 512) inner.resize(512); // guard

        if (auto bar = inner.find('|'); bar != std::string::npos)
            inner.resize(bar);

        auto ltrim = [](std::string& x)
        {
            auto p = x.find_first_not_of(" \t\r\n");
            if (p == std::string::npos)
            {
                x.clear();
                return;
            }
            x.erase(0, p);
        };
        auto rtrim = [](std::string& x)
        {
            if (x.empty()) return;
            auto p = x.find_last_not_of(" \t\r\n");
            if (p == std::string::npos)
            {
                x.clear();
                return;
            }
            x.erase(p + 1);
        };

        ltrim(inner);
        rtrim(inner);
        return inner;
    }

    std::vector<Match> find_double_brackets(const std::string& s)
    {
        static const std::regex db_re(R"(\[\[([^\[\]]+?)\]\])");
        std::vector<Match> out;
        out.reserve(16);

        for (std::sregex_iterator it(s.begin(), s.end(), db_re), end; it != end; ++it)
        {
            const auto& m = *it;
            std::size_t pos = static_cast<std::size_t>(m.position());
            std::string full = m.str(0);
            out.push_back({std::move(full), pos, static_cast<std::size_t>(m.length(0))});
            if (out.size() > 10000) break;
        }
        return out;
    }

    ParsedLinks ContentLinkParser::parse_links(const std::string& content)
    {
        ParsedLinks out;
        auto urls = find_urls(content);
        auto dbls = find_double_brackets(content);
        out.urls.reserve(urls.size());
        out.wikilinks.reserve(dbls.size());
        for (auto& m : urls) out.urls.emplace_back(std::move(m.text));
        for (auto& m : dbls)
        {
            std::string inner = m.text.substr(2, m.len - 4);
            out.wikilinks.emplace_back(wikilink_title(std::move(inner)));
        }
        return out;
    }
}
