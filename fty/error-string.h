#pragma once
#include <algorithm>
#include <array>
#include <climits>
#include <fty_log.h>
#include <pack/pack.h>
#include <string>
#include <tnt/http.h>

namespace rest {

struct WSError
{
    const char* key;       ///! short key for compile time dispatch
    int         http_code; ///! http_code is HTTP reply code, use HTTP defines
    int         err_code;  ///! sw internal error code
    const char* message;   ///! Message explaining the error, can contain printf like formatting chars
};

#define HTTP_TEAPOT 418 // see RFC2324
// clang-format off
static constexpr std::array<WSError, 18> _errors = {{
    {"undefined",                HTTP_TEAPOT,                   INT_MIN, "I'm a teapot!" },
    {"internal-error",           HTTP_INTERNAL_SERVER_ERROR,    42,      "Internal Server Error. %s" },
    {"not-authorized",           HTTP_UNAUTHORIZED,             43,      "You are not authenticated or your rights are insufficient." },
    {"element-not-found",        HTTP_NOT_FOUND,                44,      "Element '%s' not found." },
    {"method-not-allowed",       HTTP_METHOD_NOT_ALLOWED,       45,      "Http method '%s' not allowed." },
    {"request-param-required",   HTTP_BAD_REQUEST,              46,      "Parameter '%s' is required." },
    {"request-param-bad",        HTTP_BAD_REQUEST,              47,      "Parameter '%s' has bad value. Received %s. Expected %s." },
    {"bad-request-document",     HTTP_BAD_REQUEST,              48,      "Request document has invalid syntax. %s" },
    {"data-conflict",            HTTP_CONFLICT,                 50,      "Element '%s' cannot be processed because of conflict. %s" },
    {"action-forbidden",         HTTP_FORBIDDEN,                51,      "%s is forbidden. %s" },
    {"parameter-conflict",       HTTP_BAD_REQUEST,              52,      "Request cannot be processed because of conflict in parameters. %s" },
    {"content-too-big",          HTTP_REQUEST_ENTITY_TOO_LARGE, 53,      "Content size is too big, maximum size is %s." },
    {"not-found",                HTTP_NOT_FOUND,                54,      "%s does not exist." },
    {"precondition-failed",      HTTP_PRECONDITION_FAILED,      55,      "Precondition failed - %s" },
    {"db-err",                   HTTP_INTERNAL_SERVER_ERROR,    56,      "General DB error. %s" },
    {"bad-input",                HTTP_BAD_REQUEST,              57,      "Incorrect input. %s" },
    {"licensing-err",            HTTP_FORBIDDEN,                58,      "Action forbidden in current licensing state. %s" },
    {"upstream-err",             HTTP_BAD_GATEWAY,              59,      "Server which was contacted to fulfill the request has returned an error. %s" }
}};
// clang-format on
#undef HTTP_TEAPOT

struct TranslateMessage : public pack::Node
{
    pack::String    key  = FIELD("key");
    pack::StringMap vars = FIELD("variables");

    using pack::Node::Node;
    META(TranslateMessage, key, vars);
};

struct ErrorMsg
{
    std::string message;
    unsigned    code;
};

template <typename... Args>
ErrorMsg error(const std::string& key, const Args&... args)
{
    auto msg = std::find_if(_errors.begin(), _errors.end(), [&](const WSError& err) {
        return err.key == key;
    });

    if (msg == _errors.end()) {
        return {key, HTTP_INTERNAL_SERVER_ERROR};
    }

    std::string message = msg->message;
    int placeCount = 0;
    size_t pos = 0;
    while((pos = message.find("%", pos)) != std::string::npos) {
        if (pos > 0 && message[pos-1] == '\\') {
            pos++;
            continue;
        }
        if (pos < message.size() && message[pos+1] == 's') {
            std::string repl = "{{var"+std::to_string(++placeCount)+"}}";
            message = message.replace(pos, 2, repl);
            pos += repl.size() + 1;
        }
    }

    TranslateMessage tr;
    tr.key = message;

    int                      index  = 0;
    std::vector<std::string> values = {fty::convert<std::string>(args)...};
    for (const auto& str : values) {
        tr.vars.append("var" + std::to_string(++index), str);
    }

    return {*pack::json::serialize(tr), msg->http_code};
}


} // namespace rest
