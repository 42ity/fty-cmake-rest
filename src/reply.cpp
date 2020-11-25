#include "fty/rest/reply.h"
#include <tnt/httpreply.h>

namespace fty::rest {

Reply::Reply(tnt::HttpReply& reply):
    m_reply(reply)
{
}

void Reply::setContentType(const std::string& cntType)
{
    m_reply.setContentType(cntType);
}

void Reply::setHeader(const std::string& key, const std::string& value)
{
    m_reply.setHeader(key, value);
}

tnt::HttpReply& Reply::handler() const
{
    return m_reply;
}

Reply& Reply::operator<<(const std::string& cnt)
{
    m_reply.out() << cnt;
    return *this;
}

}
