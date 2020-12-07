#include "fty/rest/error.h"

namespace fty::rest {

RestError::RestError(const ErrorMsg& msg)
    : std::runtime_error(msg.message.toString())
    , m_message(msg)
{
}

const Translate& RestError::message() const
{
    return m_message.message;
}

uint16_t RestError::code() const
{
    return m_message.code;
}

}
