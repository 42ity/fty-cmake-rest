#pragma once
#include <string>
#include <functional>
#include <fty/expected.h>

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

    tnt::HttpRequest& request() const;
    tnt::QueryParams& params() const;

    const std::string& body() const;

    /// Returns variable instance from the scope by key. If such variable is not exists, this variable will be created.
    /// @param key key to get. Very weird. To be compatible with old stuff use it like `UserInfo info`
    template <typename VarT>
    VarT& global(const std::string& key) const
    {
        auto ptr = getGlobalVariable(key, []() {
            return new VarT;
        });

        return *reinterpret_cast<VarT*>(ptr);
    }

    /// Returns value from the request by it name.
    template <typename T>
    Expected<T> queryArg(const std::string& name) const
    {
        if (auto val = _queryArg(name)) {
            return convert<T>(*val);
        } else {
            return unexpected(val.error());
        }
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
    Type              m_type;
};

} // namespace fty::rest
