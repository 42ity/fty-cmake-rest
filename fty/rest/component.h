#pragma once
#include "fty/rest/error.h"
#include <tnt/componentfactory.h>
#include <tnt/ecpp.h>
#include <tnt/httpreply.h>

namespace fty::rest {

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
        } catch (const rest::Error& err) {
            reply.out() << "{ \"errors\": [ { \"message\": " << json(err.message()) << "}]}\n\n";
            return err.code();
        }
    }
};

inline std::vector<std::unique_ptr<tnt::ComponentFactory>> services;

} // namespace fty::rest


#define registerHandler(name)                                                                                          \
    static auto _init = []() {                                                                                         \
        fty::rest::services.emplace_back(                                                                              \
            std::make_unique<tnt::EcppComponentFactoryImpl<fty::rest::Rest<name>>>(name::NAME));                       \
        return true;                                                                                                   \
    }();
