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

#include "fty/rest/runner.h"
#include <tnt/httpreply.h>
#include <tnt/httprequest.h>
#include <tnt/ecpp.h>

namespace fty::rest {

// =====================================================================================================================

Runner::Runner(tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam)
    : m_request(request, qparam)
    , m_reply(reply)
{
    m_reply.setContentType("application/json;charset=UTF-8");
}

Runner::~Runner() = default;

fty::Expected<void, ErrorMsg> Runner::checkPermissions(const User::Profile& profile, const Permissions& permissions)
{
    if (permissions.count(profile) != 1) {
        return unexpected(error("Permission not defined for given profile"_tr));
    }

    Access perm = permissions.at(profile);

    if (m_request.type() == Request::Type::Get && isSet(perm, Access::Read)) {
        return {};
    }

    if (m_request.type() == Request::Type::Post  && (isSet(perm, Access::Create) || isSet(perm, Access::Edit))) {
        return {};
    }

    if (m_request.type() == Request::Type::Put  && isSet(perm, Access::Update)) {
        return {};
    }

    if (m_request.type() == Request::Type::Delete && isSet(perm, Access::Delete)) {
        return {};
    }

    return unexpected(error(ErrCode::NotAuthorized));
}

// =====================================================================================================================

} // namespace fty::rest
