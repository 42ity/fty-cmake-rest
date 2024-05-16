/*  =========================================================================
    Copyright (C) 2018 - 2020 Eaton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    =========================================================================
*/

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
