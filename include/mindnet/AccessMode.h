//
// Created by robertvokac on 9/8/25.
//
#ifndef ACCESSMODE_H
#define ACCESSMODE_H
#include "plugins/core/enums/UserRole.h"
#include "mindnet/plugins/core/enums/Crudl.h"
#include <algorithm>
#include <vector>

namespace mindnet
{
    using plugins::core::enums::Crudl;
    using plugins::core::enums::UserRole;

    enum class AccessMode
    {
        MaintenanceMode = 0,                   // API disabled: no access for any user
        AdminsReadOnly = 1,                    // Only Admins can read; all other users denied
        AdminsReadWrite = 2,                   // Only Admins can read/write; all other users denied
        AuthenticatedReadOnly = 3,             // JWT required; authenticated users can only read
        AuthenticatedReadWrite = 4,            // JWT required; authenticated users can read/write according to roles
        AuthenticatedFullAccess = 5,           // JWT required; all authenticated users have full access (roles ignored)
        PublicReadOnlyAuthenticatedReadOnly = 6,   // Guests can read; authenticated users restricted to read-only
        PublicReadOnlyAuthenticatedReadWrite = 7,  // Guests can read; authenticated users can read/write according to roles
        PublicFullAccess = 8                   // Fully open API: guests and authenticated users have unrestricted access
    };

    bool is_access_mode_in(AccessMode mode, const std::vector<AccessMode>& modes);

    std::string access_mode_to_string(const AccessMode& access_mode);

    std::string access_mode_to_string(int access_mode);

    AccessMode string_to_access_mode(const std::string& mode_str);

    mindnet::model::EnumDefinition access_mode_to_enum_definition();

    /*
 Access Control Matrix (AccessMode × Role × CRUDL)

 CRUDL shorthand:
   R = Read / List
   C = Create
   U = Update
   D = Delete

 ┌─────────────────────────────────────────────┬──────────────┬────────────┬────────────┬────────────┬─────────┐
 │ AccessMode                                  │ Admin        │ Reviewer   │ Editor     │ Reader     │ Guest   │
 ├─────────────────────────────────────────────┼──────────────┼────────────┼────────────┼────────────┼─────────┤
 │ MaintenanceMode                             │ ❌           │ ❌         │ ❌         │ ❌         │ ❌      │
 │ AdminsReadOnly                              │ R            │ ❌         │ ❌         │ ❌         │ ❌      │
 │ AdminsReadWrite                             │ R C U D      │ ❌         │ ❌         │ ❌         │ ❌      │
 │ AuthenticatedReadOnly                       │ R C U D (*)  │ R          │ R          │ R          │ ❌      │
 │ AuthenticatedReadWrite                      │ R C U D (*)  │ R C U D    │ R C U D    │ R          │ ❌      │
 │ AuthenticatedFullAccess                     │ R C U D      │ R C U D    │ R C U D    │ R C U D    │ ❌      │
 │ PublicReadOnlyAuthenticatedReadOnly         │ R C U D (*)  │ R          │ R          │ R          │ R       │
 │ PublicReadOnlyAuthenticatedReadWrite        │ R C U D (*)  │ R C U D    │ R C U D    │ R          │ R       │
 │ PublicFullAccess                            │ R C U D      │ R C U D    │ R C U D    │ R C U D    │ R C U D │
 └─────────────────────────────────────────────┴──────────────┴────────────┴────────────┴────────────┴─────────┘

 Notes:
 - Admin: In AuthenticatedReadOnly / AuthenticatedReadWrite modes, Admin always has full rights.
          (*) This means Admin ignores the "ReadOnly" restriction. Is this intended?
          If Admin should respect ReadOnly, logic should check `is_read_action`.
 - Reader: In AuthenticatedReadWrite and PublicReadOnlyAuthenticatedReadWrite, Reader only has R.
           This is consistent: Reader never modifies data.
 - Guest: Can only read in Public modes, never write, except in PublicFullAccess where Guest has full access.
 - Reviewer and Editor: Their rights are currently identical.
                        In ReadOnly modes: only R.
                        In ReadWrite modes: full R C U D.
                        If a distinction is desired, additional logic must be added.
*/

    inline bool is_read(const Crudl action) { return action == Crudl::Read || action == Crudl::List; }

    bool is_admin_authorized_to(
        const AccessMode mode,
        const Crudl action
    );

    bool is_reviewer_authorized_to(
        const AccessMode mode,
        const Crudl action
    );

    bool is_editor_authorized_to(
        AccessMode mode,
        Crudl action
    );

    bool is_reader_authorized_to(
        AccessMode mode,
        Crudl action,
        bool reader_can_write = false
    );

    bool is_guest_authorized_to(
        AccessMode mode,
        Crudl action
    );

    inline bool is_authenticated_mode(const AccessMode mode)
    {
        return mode == AccessMode::AdminsReadOnly ||
            mode == AccessMode::AdminsReadWrite ||
            mode == AccessMode::AuthenticatedReadOnly ||
            mode == AccessMode::AuthenticatedReadWrite ||
            mode == AccessMode::AuthenticatedFullAccess;
    }

    bool is_authorized_to(
        UserRole role,
        AccessMode mode,
        Crudl action,
        bool reader_can_write
    );

}
#endif // ACCESSMODE_H
