//
// Created by robertvokac on 9/8/25.
//
#ifndef REGISTRATIONMODE_H
#define REGISTRATIONMODE_H

namespace mindnet::enums
{
    enum class AccessMode
    {
        Free = 0,                   // Anybody can register, account is active immediately.
        RequiresAdminApproval = 1,  // User is registered, but must be approved by admin
        AdminAddsUsers = 2,         // Endpoint /register is disabled. Only admin can add new users.
    };
}
#endif // REGISTRATIONMODE_H
