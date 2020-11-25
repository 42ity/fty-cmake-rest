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
