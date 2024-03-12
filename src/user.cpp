#include <fty_common_rest_helpers.h>
#include "fty/rest/user.h"
#include "fty/rest/request.h"

namespace fty::rest {

User::User(const Request& req)
{
    m_info = &req.global<UserInfo>("UserInfo%user");
}

User::Profile User::profile() const
{
    if (m_info) {
        switch (m_info->profile()) {
            case BiosProfile::Admin:
                return Profile::Admin;
            case BiosProfile::Dashboard:
                return Profile::Dashboard;
            default:;
        }
    }
    return Profile::Anonymous;
}

std::string User::profileStr() const
{
    if (m_info) {
        switch (m_info->profile()) {
            case BiosProfile::Admin:
                return "Admin";
            case BiosProfile::Dashboard:
                return "Dashboard";
            default:;
        }
    }
    return "Anonymous";
}

std::string User::login() const
{
    return m_info ? m_info->login() : "Unknown";
}

}
