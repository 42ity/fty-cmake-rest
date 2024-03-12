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

#include <catch2/catch.hpp>
#include "fty/rest/translate.h"
#include <sstream>

TEST_CASE("Translate/simple")
{
    auto tr = fty::tr("this is an ex-parrot");
    CHECK("this is an ex-parrot" == tr.toString());
    CHECK(R"({"key":"this is an ex-parrot"})" == fty::rest::json(tr));
}

TEST_CASE("Translate/positional")
{
    auto tr = fty::tr("parrot: {} {}").format("norwegian", "blue");
    CHECK("parrot: norwegian blue" == tr.toString());
    CHECK(R"({"key":"parrot: {{var1}} {{var2}}","variables":{"var1":"norwegian","var2":"blue"}})" == fty::rest::json(tr));
}

TEST_CASE("Translate/indexed")
{
    auto tr = fty::tr("parrot: {1} {0}").format("norwegian", "blue");
    CHECK("parrot: blue norwegian" == tr.toString());
    CHECK(R"({"key":"parrot: {{var1}} {{var2}}","variables":{"var1":"blue","var2":"norwegian"}})" == fty::rest::json(tr));
}

TEST_CASE("Translate/named")
{
    using namespace fmt::literals;
    auto tr = fty::tr("parrot is {state}").format("state"_a = "dead");
    CHECK("parrot is dead" == tr.toString());
    CHECK(R"({"key":"parrot is {{var1}}","variables":{"var1":"dead"}})" == fty::rest::json(tr));
}

TEST_CASE("Translate/hex")
{
    auto tr = fty::tr("parrot is {:x}{:x}").format(222, 173);
    CHECK("parrot is dead" == tr.toString());
    CHECK(R"({"key":"parrot is {{var1}}{{var2}}","variables":{"var1":"de","var2":"ad"}})" == fty::rest::json(tr));
}

TEST_CASE("Translate/literal")
{
    auto tr = "parrot: {} {}"_tr.format("norwegian", "blue");
    CHECK("parrot: norwegian blue" == tr.toString());
    CHECK(R"({"key":"parrot: {{var1}} {{var2}}","variables":{"var1":"norwegian","var2":"blue"}})" == fty::rest::json(tr));
}

TEST_CASE("Translate/streaming")
{
    std::stringstream ss;
    ss << "parrot: {} {}"_tr.format("norwegian", "blue");
    CHECK("parrot: norwegian blue" == ss.str());
}

TEST_CASE("Translate/concatination")
{
    {
        std::string s = "s" + "parrot"_tr;
        CHECK("sparrot" == s);
    }
    {
        std::string s = "parrot"_tr + "s";
        CHECK("parrots" == s);
    }
}

TEST_CASE("Translate/copy")
{
    auto t1 = "RIP {}"_tr;
    t1.format("parrot");

    auto t2 = t1;

    CHECK(t1.toString() == t2.toString());

    fty::Translate t3(t1);
    CHECK(t3.toString() == t2.toString());
}
