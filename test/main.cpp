#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "fty/error-string.h"

TEST_CASE("Translate/simple")
{
    rest::Translate tr("this is an ex-parrot");
    CHECK("this is an ex-parrot" == tr.toString());
    CHECK(R"({"key": "this is an ex-parrot"})" == tr.toJson());
}

TEST_CASE("Translate/positional")
{
    rest::Translate tr("parrot: {} {}", "norwegian", "blue");
    CHECK("parrot: norwegian blue" == tr.toString());
    CHECK(R"({"key": "parrot: {{var1}} {{var2}}", "variables": {"var1": "norwegian", "var2": "blue"}})" == tr.toJson());
}

TEST_CASE("Translate/indexed")
{
    rest::Translate tr("parrot: {1} {0}", "norwegian", "blue");
    CHECK("parrot: blue norwegian" == tr.toString());
    CHECK(R"({"key": "parrot: {{var1}} {{var2}}", "variables": {"var1": "blue", "var2": "norwegian"}})" == tr.toJson());
}

TEST_CASE("Translate/named")
{
    using namespace fmt::literals;
    rest::Translate tr("parrot is {state}", "state"_a = "dead");
    CHECK("parrot is dead" == tr.toString());
    CHECK(R"({"key": "parrot is {{var1}}", "variables": {"var1": "dead"}})" == tr.toJson());
}

TEST_CASE("Translate/hex")
{
    rest::Translate tr("parrot is {:x}{:x}", 222, 173);
    CHECK("parrot is dead" == tr.toString());
    CHECK(R"({"key": "parrot is {{var1}}{{var2}}", "variables": {"var1": "de", "var2": "ad"}})" == tr.toJson());
}

