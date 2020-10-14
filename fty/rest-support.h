#pragma once
#include "error-string.h"
#include <fty/expected.h>
#include <fty/flags.h>
#include <fty_common_rest_helpers.h>
#include <fty_log.h>
#include <tnt/componentfactory.h>
#include <tnt/ecpp.h>
#include <tnt/httpreply.h>
#include <tnt/httprequest.h>

namespace rest {

// =====================================================================================================================


// =====================================================================================================================

class ErrorBase : public std::runtime_error
{
public:
    ErrorBase(ErrorMsg&& msg)
        : std::runtime_error(msg.message->toString())
        , m_message(std::move(msg))
    {
    }

    const ITranslate* message() const
    {
        return m_message.message.get();
    }

    unsigned code() const
    {
        return m_message.code;
    }

private:
    ErrorMsg m_message;
};

class Error : public ErrorBase
{
public:
    using ErrorBase::ErrorBase;

    template <typename... Args>
    Error(const std::string& msg, const Args&... args)
        : ErrorBase(rest::error(msg, args...))
    {
    }
};

// =====================================================================================================================

template <typename T>
class Rest : public tnt::EcppComponent
{
public:
    using tnt::EcppComponent::EcppComponent;

    unsigned operator()(tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam)
    {
        try {
            reply.setContentType("application/json;charset=UTF-8");
            T runner(request, reply, qparam);
            return runner.run();
        } catch (const Error& err) {
            reply.out() << err.message()->toJson() << "\n\n";
            return err.code();
        }
    }
};

} // namespace rest

// =====================================================================================================================

inline std::vector<std::unique_ptr<tnt::ComponentFactory>> services;

#define registerHandler(name)                                                                                          \
    static auto _init = []() {                                                                                         \
        services.emplace_back(std::make_unique<tnt::ComponentFactoryImpl<rest::Rest<name>>>(name::NAME));              \
        return true;                                                                                                   \
    }();

#define INIT_REST(name)                                                                                                \
    static constexpr const char* NAME = name;                                                                          \
    using rest::Runner::Runner\
