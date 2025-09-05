//
// Created by robertvokac on 9/2/25.
//
#ifndef OPERATIONRESULT_H
#define OPERATIONRESULT_H

#define operation_result mindnet::persistence::api::OperationResult
#define ok_result mindnet::persistence::api::OKOperationResult

#include <utility>

#include "mindnet/Helper.h"


namespace mindnet::persistence::api
{
    struct OperationResult
    {
        int status; // 0 = OK, other number = error
        std::string error; // error description, empty if OK

        OperationResult(int status_, std::string  error_)
            : status(status_),
              error(std::move(error_))
        {
        }

        OperationResult() : status(0)
        {
        }

        bool ok() const
        {
            return status == 0;
        }
        bool ko() const
        {
            return !ok();
        }
    };
    extern const OperationResult OKOperationResult;
}

#endif // OPERATIONRESULT_H
