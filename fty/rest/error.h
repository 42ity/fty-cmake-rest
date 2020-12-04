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

enum class ErrCode
{
    Undefined,
    InternalError,
    NotAuthorized,
    ElementNotFound,
    MethodNotAllowed,
    RequestParamRequired,
    RequestParamBad,
    BadRequestDocument,
    DataConflict,
    ActionForbidden,
    ParameterConflict,
    ContentTooBig,
    NotFound,
    PreconditionFailed,
    DbErr,
    BadInput,
    LicensingErr,
    UpstreamErr
};

template <typename... Args>
ErrorMsg error(ErrCode key, const Args&... args)
{
    struct WSError
    {
        uint16_t  http_code; ///! http_code is HTTP reply code, use HTTP defines
        int       err_code;  ///! sw internal error code
        Translate message;   ///! Message explaining the error, can contain printf like formatting chars
    };

    static constexpr unsigned HTTP_TEAPOT = 418;
    // clang-format off
    static std::map<ErrCode, WSError> errors = {{
        {ErrCode::Undefined,            {HTTP_TEAPOT,                   INT_MIN, "I'm a teapot!"_tr }},
        {ErrCode::InternalError,        {HTTP_INTERNAL_SERVER_ERROR,    42,      "Internal Server Error. {}"_tr }},
        {ErrCode::NotAuthorized,        {HTTP_UNAUTHORIZED,             43,      "You are not authenticated or your rights are insufficient."_tr }},
        {ErrCode::ElementNotFound,      {HTTP_NOT_FOUND,                44,      "Element '{}' not found."_tr }},
        {ErrCode::MethodNotAllowed,     {HTTP_METHOD_NOT_ALLOWED,       45,      "Http method '{}' not allowed."_tr }},
        {ErrCode::RequestParamRequired, {HTTP_BAD_REQUEST,              46,      "Parameter '{}' is required."_tr }},
        {ErrCode::RequestParamBad,      {HTTP_BAD_REQUEST,              47,      "Parameter '{}' has bad value. Received {}. Expected {}."_tr }},
        {ErrCode::BadRequestDocument,   {HTTP_BAD_REQUEST,              48,      "Request document has invalid syntax. {}"_tr }},
        {ErrCode::DataConflict,         {HTTP_CONFLICT,                 50,      "Element '{}' cannot be processed because of conflict. {}"_tr }},
        {ErrCode::ActionForbidden,      {HTTP_FORBIDDEN,                51,      "{} is forbidden. {}"_tr }},
        {ErrCode::ParameterConflict,    {HTTP_BAD_REQUEST,              52,      "Request cannot be processed because of conflict in parameters. {}"_tr }},
        {ErrCode::ContentTooBig,        {HTTP_REQUEST_ENTITY_TOO_LARGE, 53,      "Content size is too big, maximum size is {}."_tr }},
        {ErrCode::NotFound,             {HTTP_NOT_FOUND,                54,      "{} does not exist."_tr }},
        {ErrCode::PreconditionFailed,   {HTTP_PRECONDITION_FAILED,      55,      "Precondition failed - {}"_tr }},
        {ErrCode::DbErr,                {HTTP_INTERNAL_SERVER_ERROR,    56,      "General DB error. {}"_tr }},
        {ErrCode::BadInput,             {HTTP_BAD_REQUEST,              57,      "Incorrect input. {}"_tr }},
        {ErrCode::LicensingErr,         {HTTP_FORBIDDEN,                58,      "Action forbidden in current licensing state. {}"_tr }},
        {ErrCode::UpstreamErr,          {HTTP_BAD_GATEWAY,              59,      "Server which was contacted to fulfill the request has returned an error. {}"_tr }}
    }};
    // clang-format on

    auto msg = errors.at(key);

    return {msg.message.format(args...), msg.http_code};
}

template <typename... Args>
ErrorMsg error(const Translate& key, const Args&... args)
{
    ErrorMsg msg {key, HTTP_INTERNAL_SERVER_ERROR};
    msg.message.format(args...);
    return msg;
}

// =====================================================================================================================

class RestError : public std::runtime_error
{
public:
    RestError(const ErrorMsg& msg);
    const Translate& message() const;
    uint16_t code() const;

private:
    ErrorMsg m_message;
};

// =====================================================================================================================

class Error : public RestError
{
public:
    using RestError::RestError;

    template <typename... Args>
    Error(const Translate& msg, const Args&... args)
        : RestError(rest::error(msg, args...))
    {
    }
};

template <ErrCode Code, int ACount>
struct StdError : public Error
{
    template <typename... Args>
    StdError(const Args&... args)
        : Error(rest::error(Code, args...))
    {
        static_assert(sizeof...(args) == ACount, "Arguments count");
    }
};

// =====================================================================================================================

namespace errors {
    using Undefined            = StdError<ErrCode::Undefined, 0>;
    using Internal             = StdError<ErrCode::InternalError, 1>;
    using Unauthorized         = StdError<ErrCode::NotAuthorized, 0>;
    using ElementNotFound      = StdError<ErrCode::ElementNotFound, 1>;
    using MethodNotAllowed     = StdError<ErrCode::MethodNotAllowed, 1>;
    using RequestParamRequired = StdError<ErrCode::RequestParamRequired, 1>;
    using RequestParamBad      = StdError<ErrCode::RequestParamBad, 3>;
    using BadRequestDocument   = StdError<ErrCode::BadRequestDocument, 1>;
    using DataConflict         = StdError<ErrCode::DataConflict, 2>;
    using ActionForbidden      = StdError<ErrCode::ActionForbidden, 2>;
    using ParameterConflict    = StdError<ErrCode::ParameterConflict, 1>;
    using ContentTooBig        = StdError<ErrCode::ContentTooBig, 1>;
    using NotFound             = StdError<ErrCode::NotFound, 1>;
    using PreconditionFailed   = StdError<ErrCode::PreconditionFailed, 1>;
    using DbErr                = StdError<ErrCode::DbErr, 1>;
    using BadInput             = StdError<ErrCode::BadInput, 1>;
    using LicensingErr         = StdError<ErrCode::LicensingErr, 1>;
    using UpstreamErr          = StdError<ErrCode::UpstreamErr, 1>;
} // namespace errors

} // namespace fty::rest
