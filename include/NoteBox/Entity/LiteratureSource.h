///////////////////////////////////////////////////////////////////////////////////////////////
// note-box : Note management tool.
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
#ifndef LITERATURESOURCE_H
#define LITERATURESOURCE_H

#include <ostream>
#include <string>
#include "NoteBox/Helper.h"

namespace NoteBox::Entity {
    using std::string;

    struct LiteratureSource {
        int id;
        string title;
        string author;
        ushort year;
        string publication;
        string url;
        string content;

        LiteratureSource(int id_, string& title_, string& author_, ushort year_, string& publication_,
                         string& url_, string& content_) : id(id_),
                                                                         title(std::move(title_)),
                                                                         author(std::move(author_)),
                                                                         year(year_),
                                                                         publication(std::move(publication_)),
                                                                         url(std::move(url_)),
                                                                         content(std::move(content_)) {
        }

        friend std::ostream &operator<<(std::ostream &os, const LiteratureSource &source) {
            os << "LiteratureSource{id: " << source.id
                    << ", title: " << source.title
                    << ", author: " << source.author
                    << ", year: " << source.year
                    << ", publication: " << source.publication
                    << ", url: " << source.url
                    << "}";
            return os;
        }

        bool operator==(const LiteratureSource &other) const {
            return this->id == other.id &&
                   this->title == other.title &&
                   this->author == other.author;
        }
    };
}

#endif // LITERATURESOURCE_H
