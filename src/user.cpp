/*  =========================================================================
    Copyright (C) 2018 - 2020 Eaton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    =========================================================================
*/

#include <fty_common_rest_helpers.h>
#include "fty/rest/user.h"
#include "fty/rest/request.h"

namespace fty::rest {

User::User(const Request& req)
{
    m_info = &req.global<UserInfo>("UserInfo%user");
}

User::Profile User::profile() const
{
    if (m_info) {
        switch (m_info->profile()) {
            case BiosProfile::Admin:
                return Profile::Admin;
            case BiosProfile::Dashboard:
                return Profile::Dashboard;
            default:;
        }
    }
    return Profile::Anonymous;
}

std::string User::profileStr() const
{
    if (m_info) {
        switch (m_info->profile()) {
            case BiosProfile::Admin:
                return "Admin";
            case BiosProfile::Dashboard:
                return "Dashboard";
            default:;
        }
    }
    return "Anonymous";
}

std::string User::login() const
{
    return m_info ? m_info->login() : "Unknown";
}

}
