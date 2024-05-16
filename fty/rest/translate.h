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

inline void json(const Translate& trans, details::TranslateMessage& out)
{
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
        out.vars.append(name, match[1].str());
        view = match.suffix();
    }
    res += view;
    out.key = res;
}

inline std::string json(const Translate& trans)
{
    details::TranslateMessage tr;
    json(trans, tr);
    return *pack::json::serialize(tr);
}

} // namespace fty::rest
