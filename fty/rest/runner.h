#pragma once
#include <fty/flags.h>
#include <fty/expected.h>
#include <map>
#include "fty/error-string.h"

namespace tnt {
class HttpRequest;
class HttpReply;
class QueryParams;
} // namespace tnt

namespace fty::rest {

enum Access
{
    Read   = 1 << 0,
    Create = 1 << 1,
    Update = 1 << 2,
    Edit   = 1 << 3,
    Delete = 1 << 4
};
ENABLE_FLAGS(Access)

enum Profile
{
    Anonymous = -1, // Not authorized users
    Dashboard = 0,  // Dashboard profile
    Admin     = 3   // Admin profile
};

class Runner
{
public:
    using Permissions = std::map<Profile, Access>;

public:
    Runner(tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam);
    virtual ~Runner();
    virtual unsigned run() = 0;

protected:
    template <typename VarT>
    fty::Expected<VarT*> global(const std::string& key) const
    {
        VarT*       var   = nullptr;
        tnt::Scope& scope = m_request.getRequestScope();
        var               = scope.get<VarT>(key);

        if (!var) {
            var = new VarT;
            scope.put<VarT>(key, var);
        }

        unsigned r = tnt::controllerCaller(var, m_request, m_reply, m_qparam);
        if (r != DECLINED && r != HTTP_OK) {
            return fty::unexpected("Cannot initialize variable");
        }

        return var;
    }

    Expected<void, ErrorMsg> checkPermissions(const Profile& profile, const Permissions& permissions);

    template <typename T, bool required = false>
    fty::Expected<T, ErrorMsg> queryValue(const std::string& name) const
    {
        if (auto val = _queryValue(name)) {
            return convert<T>(*val);
        } else if (required) {
            return fty::unexpected(error("request-param-required", name));
        }
        return T{};
    }

    template <typename... Args>
    unsigned handleError(const std::string& msg, const Args&... args)
    {
        auto err = rest::error(msg, args...);
        m_reply.out() << err.message << "\n\n";
        return err.code;
    }
private:
    Expected<std::string> _queryValue(const std::string& name) const;

protected:
    tnt::HttpRequest& m_request;
    tnt::HttpReply&   m_reply;
    tnt::QueryParams& m_qparam;
};

} // namespace fty::rest
