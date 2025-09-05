//
// Created by robertvokac on 9/2/25.
//
#ifndef CRUDLVALIDATORRESULT_H
#define CRUDLVALIDATORRESULT_H
#define validator_result mindnet::persistence::api::ValidatorResult
#define ok_result mindnet::persistence::api::EmptyValidatorResult

#include <utility>

#include "mindnet/Helper.h"


namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::api
{
    struct ValidatorResult
    {
        int status; // 0 = OK, other number = error
        std::string error; // error description, empty if OK
        ValidatorResult(int status_, std::string  error_)
            : status(status_),
              error(std::move(error_))
        {
        }

        ValidatorResult() : status(0)
        {
        }

    };
    extern const ValidatorResult EmptyValidatorResult;
}


#endif // CRUDLVALIDATORRESULT_H
