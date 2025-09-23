//
// Created by robertvokac on 9/6/25.
//
#ifndef CHATPERSISTENCEMETHODS_H
#define CHATPERSISTENCEMETHODS_H


#include <utility>

#include "mindnet/api/PersistenceMethods.h"

#include "mindnet/api/RequestContext.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/core/models/Team.h"
#include "mindnet/plugins/core/models/TeamMember.h"
#include "mindnet/plugins/chat/models/Discussion.h"
#include "mindnet/plugins/chat/models/Comment.h"

namespace mindnet::plugins::core::models
{
    struct User;
}

namespace mindnet::plugins::chat
{
    gen_find_h(chat, Comment, comment)
    gen_find_h(chat, Discussion, discussion)


}

#endif // CHATPERSISTENCEMETHODS_H
