#include "fty/rest/user.h"
#include <fty_common_rest_helpers.h>
#include "fty/rest/request.h"

namespace fty::rest {

User::User(const Request& req)
{
    m_info = &req.global<UserInfo>("UserInfo user");
}

User::Profile User::profile() const
{
    assert(m_info);
    switch (m_info->profile()) {
    case BiosProfile::Anonymous:
        return Profile::Anonymous;
    case BiosProfile::Admin:
        return Profile::Admin;
    case BiosProfile::Dashboard:
        return Profile::Dashboard;
    }
    return Profile::Anonymous;
}

std::string User::login() const
{
    assert(m_info);
    return m_info->login();
}

}
