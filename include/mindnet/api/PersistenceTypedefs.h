//
// Created by robertvokac on 9/7/25.
//

#ifndef MIND_NET_PERSISTENCETYPEDEFS_H
#define MIND_NET_PERSISTENCETYPEDEFS_H
#include <expected>

#include "../OperationResult.h"

namespace mindnet::api
{
    typedef std::expected<void, OperationResult> result_t;
}

#endif //MIND_NET_PERSISTENCETYPEDEFS_H
