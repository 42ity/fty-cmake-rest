#pragma once
#include "fty/rest/translate.h"
#include <climits>
#include <tnt/http.h>

namespace fty::rest {

// =====================================================================================================================

struct ErrorMsg
{
    ErrorMsg(const Translate& tr, uint16_t _code)
        : message(tr)
        , code(_code)
    {
    }

    Translate message;
    uint16_t  code;

    ErrorMsg(const ErrorMsg&) = default;
    ErrorMsg(ErrorMsg&&)      = default;
    ErrorMsg& operator=(const ErrorMsg&) = default;
    ErrorMsg& operator=(ErrorMsg&&) = default;
};

// =====================================================================================================================

template <typename... Args>
ErrorMsg error(const std::string& key, const Args&... args)
{
    struct WSError
    {
        const char* key;       ///! short key for compile time dispatch
        uint16_t    http_code; ///! http_code is HTTP reply code, use HTTP defines
        int         err_code;  ///! sw internal error code
        Translate   message;   ///! Message explaining the error, can contain printf like formatting chars
    };

    static constexpr unsigned HTTP_TEAPOT = 418;
    // clang-format off
    static std::vector<WSError> errors = {{
        {"undefined",                HTTP_TEAPOT,                   INT_MIN, "I'm a teapot!"_tr },
        {"internal-error",           HTTP_INTERNAL_SERVER_ERROR,    42,      "Internal Server Error. {}"_tr },
        {"not-authorized",           HTTP_UNAUTHORIZED,             43,      "You are not authenticated or your rights are insufficient."_tr },
        {"element-not-found",        HTTP_NOT_FOUND,                44,      "Element '{}' not found."_tr },
        {"method-not-allowed",       HTTP_METHOD_NOT_ALLOWED,       45,      "Http method '{}' not allowed."_tr },
        {"request-param-required",   HTTP_BAD_REQUEST,              46,      "Parameter '{}' is required."_tr },
        {"request-param-bad",        HTTP_BAD_REQUEST,              47,      "Parameter '{}' has bad value. Received {}. Expected {}."_tr },
        {"bad-request-document",     HTTP_BAD_REQUEST,              48,      "Request document has invalid syntax. {}"_tr },
        {"data-conflict",            HTTP_CONFLICT,                 50,      "Element '{}' cannot be processed because of conflict. {}"_tr },
        {"action-forbidden",         HTTP_FORBIDDEN,                51,      "{} is forbidden. {}"_tr },
        {"parameter-conflict",       HTTP_BAD_REQUEST,              52,      "Request cannot be processed because of conflict in parameters. {}"_tr },
        {"content-too-big",          HTTP_REQUEST_ENTITY_TOO_LARGE, 53,      "Content size is too big, maximum size is {}."_tr },
        {"not-found",                HTTP_NOT_FOUND,                54,      "{} does not exist."_tr },
        {"precondition-failed",      HTTP_PRECONDITION_FAILED,      55,      "Precondition failed - {}"_tr },
        {"db-err",                   HTTP_INTERNAL_SERVER_ERROR,    56,      "General DB error. {}"_tr },
        {"bad-input",                HTTP_BAD_REQUEST,              57,      "Incorrect input. {}"_tr },
        {"licensing-err",            HTTP_FORBIDDEN,                58,      "Action forbidden in current licensing state. {}"_tr },
        {"upstream-err",             HTTP_BAD_GATEWAY,              59,      "Server which was contacted to fulfill the request has returned an error. {}"_tr }
    }};
    // clang-format on

    auto msg = std::find_if(errors.begin(), errors.end(), [&](const WSError& err) {
        return err.key == key;
    });

    if (msg == errors.end()) {
        return {Translate(key).format(args...), HTTP_INTERNAL_SERVER_ERROR};
    }

    return {msg->message.format(args...), msg->http_code};
}

template <typename... Args>
ErrorMsg error(Translate& key, const Args&... args)
{
    return {key.format(args...), HTTP_INTERNAL_SERVER_ERROR};
}

// =====================================================================================================================

class ErrorBase : public std::runtime_error
{
public:
    ErrorBase(const ErrorMsg& msg)
        : std::runtime_error(msg.message.toString())
        , m_message(msg)
    {
    }

    const Translate& message() const
    {
        return m_message.message;
    }

    uint16_t code() const
    {
        return m_message.code;
    }

private:
    ErrorMsg m_message;
};

// =====================================================================================================================

class Error : public ErrorBase
{
public:
    using ErrorBase::ErrorBase;

    template <typename... Args>
    Error(const std::string& msg, const Args&... args)
        : ErrorBase(rest::error(msg, args...))
    {
    }

    template <typename... Args>
    Error(Translate& msg, const Args&... args)
        : ErrorBase(rest::error(msg, args...))
    {
    }
};

// =====================================================================================================================

} // namespace fty::rest
