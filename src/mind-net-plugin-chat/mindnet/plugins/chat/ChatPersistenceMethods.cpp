//
// Created by robertvokac on 9/6/25.
//

#include "mindnet/plugins/chat/ChatPersistenceMethods.h"

#include "mindnet/api/IService.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/chat/models/Comment.h"
#include "mindnet/plugins/chat/models/Discussion.h"

namespace mindnet::plugins::chat
{
    gen_find_cpp(chat, Comment, comment, COMMENT)
    gen_find_cpp(chat, Discussion, discussion, DISCUSSION)

}
