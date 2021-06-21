#include "fty/rest/request.h"
#include <tnt/httprequest.h>
#include <tnt/query_params.h>

namespace fty::rest {

Request::Request(tnt::HttpRequest& request, tnt::QueryParams& params)
    : m_request(request)
    , m_params(params)
{
    if (m_request.isMethodGET()) {
        m_type = Type::Get;
    }

    if (m_request.isMethodPOST()) {
        m_type = Type::Post;
    }

    if (m_request.getMethod() == "PUT") {
        m_type = Type::Put;
    }

    if (m_request.getMethod() == "DELETE") {
        m_type = Type::Delete;
    }

    if (m_request.isMethodHEAD()) {
        m_type = Type::Head;
    }
}

Request::Type Request::type() const
{
    return m_type;
}

tnt::HttpRequest& Request::request() const
{
    return m_request;
}

tnt::QueryParams& Request::params() const
{
    return m_params;
}

const std::string& Request::body() const
{
    return m_request.getBody();
}

void* Request::getGlobalVariable(const std::string& name, std::function<void*()>&& createFunc) const
{
    tnt::Scope& scope = m_request.getRequestScope();
    void*       var   = scope.get<void*>(name);

    if (!var) {
        var = createFunc();
        scope.put(name, var);
    }

    return var;
}

Expected<std::string> Request::_queryArg(const std::string& name) const
{
    if (m_params.has(name)) {
        std::string strVal = m_params.param(name);
        return Expected<std::string>(strVal);
    } else {
        const auto& args = m_request.getArgs();
        if (args.find(name) == args.end()){
            return unexpected("{} not exists", name);
        }
        std::string strVal = args.at(name);
        return Expected<std::string>(strVal);
    }
}

size_t Request::contentSize() const
{
    return m_request.getContentSize();
}

Expected<std::string> Request::multipart(const std::string& name) const
{
    const tnt::Multipart& mp = m_request.getMultipart();
    auto                  it = mp.find(name);
    if (it == mp.end()) {
        return unexpected("No such {} miltipart", name);
    }

    return it->getBody();
}

std::string Request::path() const
{
    return m_request.getPathInfo();
}

} // namespace fty::rest
