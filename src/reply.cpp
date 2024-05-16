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

#include "fty/rest/reply.h"
#include <tnt/httpreply.h>

namespace fty::rest {

Reply::Reply(tnt::HttpReply& reply):
    m_reply(reply)
{
}

void Reply::setContentType(const std::string& cntType)
{
    m_reply.setContentType(cntType);
}

void Reply::setHeader(const std::string& key, const std::string& value)
{
    m_reply.setHeader(key, value);
}

tnt::HttpReply& Reply::handler() const
{
    return m_reply;
}

Reply& Reply::operator<<(const std::string& cnt)
{
    m_reply.out() << cnt;
    return *this;
}

}
