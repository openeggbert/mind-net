///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : Mind map software.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see 
// <https://www.gnu.org/licenses/> or write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CONTENTFORMAT_H
#define CONTENTFORMAT_H

#include <string>

namespace mindnet::plugins::zettelkasten::enums
{
    /**
     *
     * @author robertvokac
     */
    enum class ContentFormat
    {
        MARKDOWN = 0, HTML = 1, PLAIN = 2
    };

    inline std::string content_format_to_string(ContentFormat format)
    {
        switch (format)
        {
        case ContentFormat::MARKDOWN:
            return "Markdown";
        case ContentFormat::HTML:
            return "HTML";
        case ContentFormat::PLAIN:
            return "Plain";
        default:
            return "Unknown";
        }
    }

    inline std::string content_format_to_string(int format)
    {
        return content_format_to_string(static_cast<ContentFormat>(format));
    }

    inline mindnet::model::EnumDefinition content_format_to_enum_definition()
    {
        return mindnet::model::EnumDefinition{
            content_format_to_string, 3, 0, 1, 2
        };
    }
} // namespace mindnet::enums
#endif // CONTENTFORMAT_H
