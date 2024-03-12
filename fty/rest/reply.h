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

namespace tnt {
class HttpReply;
}

namespace fty::rest {

class Reply
{
public:
    void setContentType(const std::string& cntType);
    void setHeader(const std::string& key, const std::string& value);
    tnt::HttpReply& handler() const;

    Reply& operator<<(const std::string& cnt);
private:
    Reply(tnt::HttpReply& reply);
    friend class Runner;

private:
    tnt::HttpReply& m_reply;
};

}
