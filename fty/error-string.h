#pragma once
#include "fty/rest/translate.h"
#include <array>
#include <climits>
#include <string>
#include <tnt/http.h>

#define TRANSLATE_ME(str) str

namespace fty::rest {

// =====================================================================================================================

struct ErrorMsg
{
    ErrorMsg(ITranslate* tr, unsigned _code)
        : message(tr)
        , code(_code)
    {
    }

    std::unique_ptr<ITranslate> message;
    unsigned                    code;

    ErrorMsg(const ErrorMsg&) = delete;
    ErrorMsg(ErrorMsg&&)      = default;
    ErrorMsg& operator=(const ErrorMsg&) = delete;
    ErrorMsg& operator=(ErrorMsg&&) = default;
};

// =====================================================================================================================

template <typename... Args>
ErrorMsg error(const std::string& key, const Args&... args)
{
    struct WSError
    {
        const char* key;       ///! short key for compile time dispatch
        int         http_code; ///! http_code is HTTP reply code, use HTTP defines
        int         err_code;  ///! sw internal error code
        const char* message;   ///! Message explaining the error, can contain printf like formatting chars
    };

    static constexpr unsigned HTTP_TEAPOT = 418;
    // clang-format off
    static constexpr std::array<WSError, 18> errors = {{
        {"undefined",                HTTP_TEAPOT,                   INT_MIN, TRANSLATE_ME("I'm a teapot!") },
        {"internal-error",           HTTP_INTERNAL_SERVER_ERROR,    42,      TRANSLATE_ME("Internal Server Error. {}") },
        {"not-authorized",           HTTP_UNAUTHORIZED,             43,      TRANSLATE_ME("You are not authenticated or your rights are insufficient.") },
        {"element-not-found",        HTTP_NOT_FOUND,                44,      TRANSLATE_ME("Element '{}' not found.") },
        {"method-not-allowed",       HTTP_METHOD_NOT_ALLOWED,       45,      TRANSLATE_ME("Http method '{}' not allowed.") },
        {"request-param-required",   HTTP_BAD_REQUEST,              46,      TRANSLATE_ME("Parameter '{}' is required.") },
        {"request-param-bad",        HTTP_BAD_REQUEST,              47,      TRANSLATE_ME("Parameter '{}' has bad value. Received {}. Expected {}.") },
        {"bad-request-document",     HTTP_BAD_REQUEST,              48,      TRANSLATE_ME("Request document has invalid syntax. {}") },
        {"data-conflict",            HTTP_CONFLICT,                 50,      TRANSLATE_ME("Element '{}' cannot be processed because of conflict. {}") },
        {"action-forbidden",         HTTP_FORBIDDEN,                51,      TRANSLATE_ME("{} is forbidden. {}") },
        {"parameter-conflict",       HTTP_BAD_REQUEST,              52,      TRANSLATE_ME("Request cannot be processed because of conflict in parameters. {}") },
        {"content-too-big",          HTTP_REQUEST_ENTITY_TOO_LARGE, 53,      TRANSLATE_ME("Content size is too big, maximum size is {}.") },
        {"not-found",                HTTP_NOT_FOUND,                54,      TRANSLATE_ME("{} does not exist.") },
        {"precondition-failed",      HTTP_PRECONDITION_FAILED,      55,      TRANSLATE_ME("Precondition failed - {}") },
        {"db-err",                   HTTP_INTERNAL_SERVER_ERROR,    56,      TRANSLATE_ME("General DB error. {}") },
        {"bad-input",                HTTP_BAD_REQUEST,              57,      TRANSLATE_ME("Incorrect input. {}") },
        {"licensing-err",            HTTP_FORBIDDEN,                58,      TRANSLATE_ME("Action forbidden in current licensing state. {}") },
        {"upstream-err",             HTTP_BAD_GATEWAY,              59,      TRANSLATE_ME("Server which was contacted to fulfill the request has returned an error. {}") }
    }};
    // clang-format on

    auto msg = std::find_if(errors.begin(), errors.end(), [&](const WSError& err) {
        return err.key == key;
    });

    if (msg == errors.end()) {
        return {new Translate(key, args...), HTTP_INTERNAL_SERVER_ERROR};
    }

    return {new Translate(msg->message, args...), msg->http_code};
}

// =====================================================================================================================

} // namespace fty::rest
