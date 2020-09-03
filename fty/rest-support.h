#pragma once
#include <fty/expected.h>
#include <fty_common_rest_helpers.h>
#include <fty_log.h>
#include <tnt/componentfactory.h>
#include <tnt/ecpp.h>
#include <tnt/httpreply.h>
#include <tnt/httprequest.h>
#include "error-string.h"
#include <fty/flags.h>

namespace rest {

// =====================================================================================================================

enum Access
{
    Read,
    Create,
    Update,
    Edit,
    Delete
};

ENABLE_FLAGS(Access)

class Runner
{
public:
    using Permissions = std::map<BiosProfile, Access>;

public:
    Runner(tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam)
        : m_request(request)
        , m_reply(reply)
        , m_qparam(qparam)
    {
        m_reply.setContentType("application/json;charset=UTF-8");
    }
    virtual ~Runner() = default;

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

    fty::Expected<void, Error> checkPermissions(const UserInfo& user, const Permissions& permissions)
    {
        if (permissions.count(user.profile()) != 1) {
            return fty::unexpected(error("Permission not defined for given profile"));
        }

        Access perm = permissions.at(user.profile());

        if (m_request.isMethodGET() && fty::isSet(perm, Access::Read)) {
            return {};
        }

        if (m_request.isMethodPOST() && (fty::isSet(perm, Access::Create) || fty::isSet(perm, Access::Edit))) {
            return {};
        }

        if (m_request.getMethod() == "PUT" && fty::isSet(perm, Access::Update)) {
            return {};
        }

        if (m_request.getMethod() == "DELETE" && fty::isSet(perm, Access::Delete)) {
            return {};
        }

        return fty::unexpected(error("not-authorized"));
    }

    template<typename T, bool required = false>
    fty::Expected<T, Error> queryValue(const std::string& name) const
    {
        if (m_qparam.has(name)) {
            std::string strVal = m_qparam.param(name);
            return fty::convert<T>(strVal);
        }
        if (required) {
            return fty::unexpected(error("request-param-required", name.c_str()));
        }
        return T{};
    }
protected:
    tnt::HttpRequest& m_request;
    tnt::HttpReply&   m_reply;
    tnt::QueryParams& m_qparam;
};

// =====================================================================================================================

template <typename T>
class Rest : public tnt::EcppComponent
{
public:
    using tnt::EcppComponent::EcppComponent;

    unsigned operator()(tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam)
    {
        T runner(request, reply, qparam);
        return runner.run();
    }
};

} // namespace rest

// =====================================================================================================================

inline std::vector<std::unique_ptr<tnt::ComponentFactory>> services;

#define registerHandler(name)                                                                                          \
    static auto _init = []() {                                                                                         \
        services.emplace_back(std::make_unique<tnt::ComponentFactoryImpl<rest::Rest<name>>>(name::NAME));              \
        return true;                                                                                                   \
    }();
