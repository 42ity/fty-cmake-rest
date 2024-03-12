#pragma once
#include <string>

class UserInfo;

namespace fty::rest {

class Request;

class User
{
public:
    enum class Profile
    {
        Anonymous = -1, // Not authorized users
        Dashboard = 0,  // Dashboard profile
        Admin     = 3   // Admin profile
    };

public:
    User(const Request& req);

    std::string login() const;

    Profile     profile() const;
    std::string profileStr() const;

private:
    UserInfo* m_info{nullptr};
};

} // namespace fty::rest
