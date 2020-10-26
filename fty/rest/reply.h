#pragma once
#include <string>

namespace tnt {
class HttpReply;
}

namespace fty::rest {

class Reply
{
public:
    void setContentType(const std::string& cntType);
    tnt::HttpReply& handler() const;

    Reply& operator<<(const std::string& cnt);
private:
    Reply(tnt::HttpReply& reply);
    friend class Runner;

private:
    tnt::HttpReply& m_reply;
};

}
