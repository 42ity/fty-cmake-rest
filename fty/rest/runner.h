#pragma once
#include "fty/rest/error.h"
#include "fty/rest/reply.h"
#include "fty/rest/request.h"
#include "fty/rest/user.h"
#include <fty/expected.h>
#include <fty/flags.h>
#include <map>

namespace tnt {
class HttpRequest;
class QueryParams;
class Scope;
} // namespace tnt

namespace fty::rest {

// =====================================================================================================================

enum class Access
{
    Read   = 1 << 0,
    Create = 1 << 1,
    Update = 1 << 2,
    Edit   = 1 << 3,
    Delete = 1 << 4
};

ENABLE_FLAGS(Access)

// =====================================================================================================================

class Runner
{
public:
    using Permissions = std::map<User::Profile, Access>;

public:
    Runner(tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam);
    virtual ~Runner();
    virtual unsigned run() = 0;

protected:
    fty::Expected<void, ErrorMsg> checkPermissions(const User::Profile& profile, const Permissions& permissions);

protected:
    Request m_request;
    Reply   m_reply;
};

// =====================================================================================================================

} // namespace fty::rest

#define INIT_REST(name)                                                                                                \
    static constexpr const char* NAME = name;                                                                          \
    using rest::Runner::Runner
