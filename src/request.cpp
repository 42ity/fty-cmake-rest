#include "fty/rest/request.h"
#include <tnt/httprequest.h>
#include <tnt/query_params.h>

namespace fty::rest {

Request::Request(tnt::HttpRequest& request, tnt::QueryParams& params):
    m_request(request),
    m_params(params)
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
    void* var = scope.get<void*>(name);

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
        return std::move(strVal);
    }
    return unexpected("{} not exists", name);
}

}
