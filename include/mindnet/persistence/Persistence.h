//
// Created by robertvokac on 8/6/25.
//
#ifndef DB_H
#define DB_H

#include "api/ContentRepository.h"
#include "api/MapRepository.h"


namespace mindnet::persistence {
    class Persistence {
    public:
        persistence::api::ContentRepository* content_repository{};
        persistence::api::MapRepository* map_repository{};

        Persistence();
    };

}
#endif // DB_H
