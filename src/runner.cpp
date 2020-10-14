#include "fty/rest/runner.h"
#include <tnt/httpreply.h>
#include <tnt/httprequest.h>


namespace fty::rest {

Runner::Runner(tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam)
    : m_request(request)
    , m_reply(reply)
    , m_qparam(qparam)
{
    m_reply.setContentType("application/json;charset=UTF-8");
}

Runner::~Runner() = default;

fty::Expected<void, ErrorMsg> Runner::checkPermissions(const Profile& profile, const Permissions& permissions)
{
    if (permissions.count(profile) != 1) {
        return unexpected(error("Permission not defined for given profile"));
    }

    Access perm = permissions.at(profile);

    if (m_request.isMethodGET() && isSet(perm, Access::Read)) {
        return {};
    }

    if (m_request.isMethodPOST() && (isSet(perm, Access::Create) || isSet(perm, Access::Edit))) {
        return {};
    }

    if (m_request.getMethod() == "PUT" && isSet(perm, Access::Update)) {
        return {};
    }

    if (m_request.getMethod() == "DELETE" && isSet(perm, Access::Delete)) {
        return {};
    }

    return unexpected(error("not-authorized"));
}

Expected<std::string> Runner::_queryValue(const std::string& name) const
{
    if (m_qparam.has(name)) {
        std::string strVal = m_qparam.param(name);
        return std::move(strVal);
    }
    return unexpected("not exists");
}

}
