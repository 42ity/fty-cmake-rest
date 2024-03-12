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

#pragma once

#include <string>

class UserInfo;

namespace fty::rest {

class Request;

class User
{
public:
    enum class Profile
    {
        Anonymous = -1, // Not authorized users
        Dashboard = 0,  // Dashboard profile
        Admin     = 3   // Admin profile
    };

public:
    User(const Request& req);

    std::string login() const;

    Profile     profile() const;
    std::string profileStr() const;

private:
    UserInfo* m_info{nullptr};
};

} // namespace fty::rest
