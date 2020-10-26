#pragma once
#include <fty/translate.h>
#include <pack/pack.h>
#include <regex>


namespace fty::rest {

namespace details {
    /// Translate message structure
    struct TranslateMessage : public pack::Node
    {
        pack::String    key  = FIELD("key");
        pack::StringMap vars = FIELD("variables");

        using pack::Node::Node;
        META(TranslateMessage, key, vars);
    };
} // namespace details

inline std::string json(const Translate& trans)
{
    details::TranslateMessage tr;

    static std::regex rex("\\{([^\\}]*)\\}");
    static std::regex rex2("@\\[([^(\\]@)]*)\\]@");

    std::smatch match;
    std::string view(trans.message());
    std::string newmsg;
    while (std::regex_search(view, match, rex)) {
        newmsg += match.prefix().str() + "@[{" + match[1].str() + "}]@";
        view = match.suffix();
    }
    newmsg += view;

    std::string res;
    view      = trans.formatFunc()(newmsg);
    int index = 0;
    while (std::regex_search(view, match, rex2)) {
        std::string name = "var" + std::to_string(++index);
        res += match.prefix().str() + "{{" + name + "}}";
        tr.vars.append(name, match[1].str());
        view = match.suffix();
    }
    res += view;
    tr.key = res;

    return *pack::json::serialize(tr);
}

} // namespace fty::rest
