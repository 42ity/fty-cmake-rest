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

#include <string>
#include <functional>
#include <fty/expected.h>
#include <tnt/httprequest.h>

namespace tnt {
class HttpRequest;
class QueryParams;
} // namespace tnt

namespace fty::rest {

class Request
{
public:
    enum class Type
    {
        Get,
        Post,
        Head,
        Put,
        Delete
    };

    Type type() const;
    std::string typeStr() const;

    tnt::HttpRequest& request() const;
    tnt::QueryParams& params() const;

    const std::string& body() const;

    /// Returns variable instance from the scope by key. If such variable don't exists, this variable will be created.
    /// @param key key to get. Very weird. To be compatible with old stuff use it like `UserInfo info`
    template <typename VarT>
    VarT& global(const std::string& key) const
    {
        auto ptr = getGlobalVariable(key, []() {
            return new VarT;
        });

        return *reinterpret_cast<VarT*>(ptr);
    }

    /// Returns value from the request by its name.
    template <typename T>
    Expected<T> queryArg(const std::string& name) const
    {
        if (auto val = _queryArg(name)) {
            return convert<T>(*val);
        }
        else {
            return unexpected(val.error());
        }
    }

    bool isParamsEmpty() {
       return m_params.empty();
    }

    size_t contentSize() const;
    Expected<std::string> multipart(const std::string& name) const;
    std::string path() const;

private:
    Request(tnt::HttpRequest& request, tnt::QueryParams& params);
    void* getGlobalVariable(const std::string& name, std::function<void*()>&& createFunc) const;
    Expected<std::string> _queryArg(const std::string& name) const;

    friend class Runner;

private:
    tnt::HttpRequest& m_request;
    tnt::QueryParams& m_params;
    Type              m_type{Type::Get};
};

} // namespace fty::rest
