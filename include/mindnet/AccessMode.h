//
// Created by robertvokac on 9/8/25.
//

#ifndef MIND_NET_ACCESSMODE_H
namespace mindnet
{
    enum class AccessMode
    {
        AuthenticatedOnly = 0,            // All requests require JWT
        UnauthenticatedCanRead = 1,  // GET is public, others require JWT
        EveryoneCanDoEverything = 2, // No authentication at all. New user everybody is created and used for this purpose
        MaintenanceMode = 10         // API is disabled, only admins can access
    };
}
#define MIND_NET_ACCESSMODE_H

#endif //MIND_NET_ACCESSMODE_H