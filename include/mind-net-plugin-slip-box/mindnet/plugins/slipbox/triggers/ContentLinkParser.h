//
// Created by robertvokac on 10/23/25.
//

#ifndef MIND_NET_CONTENTLINKPARSER_H
#define MIND_NET_CONTENTLINKPARSER_H
#include <string>
#include <vector>


namespace mindnet::plugins::slipbox::triggers
{
    struct Match
    {
        std::string text;
        std::size_t pos;
        std::size_t len;
    };

    struct ParsedLinks
    {
        std::vector<std::string> urls;
        std::vector<std::string> wikilinks;
    };

    class ContentLinkParser
    {
    public:
        static ParsedLinks parse_links(const std::string& content);
    };
}
#endif //MIND_NET_CONTENTLINKPARSER_H
