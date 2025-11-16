#include <variant>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <limits>

#include "BencodeParser.hpp"

using namespace bt;

//////////////////
// Integer tests
//////////////////
TEST_CASE("Parse integer 1") {
    auto res = bencode::parse("i123e");
    REQUIRE(std::holds_alternative<int64_t>(res.value));
    REQUIRE(std::get<int64_t>(res.value) == 123);
}

TEST_CASE("Parse integer 2") {
    auto res = bencode::parse("i-42e");
    REQUIRE(std::holds_alternative<int64_t>(res.value));
    REQUIRE(std::get<int64_t>(res.value) == -42);
}

TEST_CASE("Parse integer 3") {
    auto res = bencode::parse("i0e");
    REQUIRE(std::holds_alternative<int64_t>(res.value));
    REQUIRE(std::get<int64_t>(res.value) == 0);
}

TEST_CASE("Parse integer 4") {
    auto res = bencode::parse("i12093810981093e");
    REQUIRE(std::holds_alternative<int64_t>(res.value));
    REQUIRE(std::get<int64_t>(res.value) == 12093810981093);
}

TEST_CASE("Parse integer int64 max") {
    auto res = bencode::parse("i9223372036854775807e");
    REQUIRE(std::holds_alternative<int64_t>(res.value));
    REQUIRE(std::get<int64_t>(res.value) == std::numeric_limits<int64_t>::max());
}

TEST_CASE("Parse integer int64 min") {
    auto res = bencode::parse("i-9223372036854775808e");
    REQUIRE(std::holds_alternative<int64_t>(res.value));
    REQUIRE(std::get<int64_t>(res.value) == std::numeric_limits<int64_t>::min());
}

TEST_CASE("Parse integer negative zero") {
    REQUIRE_THROWS_AS(bencode::parse("i-0e"), std::invalid_argument);
}

TEST_CASE("Parse integer leading zero") {
    REQUIRE_THROWS_AS(bencode::parse("i0123e"), std::invalid_argument);
}

TEST_CASE("Parse integer missing end") {
    REQUIRE_THROWS_AS(bencode::parse("i123"), std::invalid_argument);
}

TEST_CASE("Parse integer empty") {
    REQUIRE_THROWS_AS(bencode::parse("ie"), std::invalid_argument);
}

TEST_CASE("Parse integer overflow") {
    REQUIRE_THROWS_AS(bencode::parse("i9223372036854775808e"), std::out_of_range);
}

TEST_CASE("Parse integer underflow") {
    REQUIRE_THROWS_AS(bencode::parse("i-9223372036854775809e"), std::out_of_range);
}

TEST_CASE("Parse invalid integer") {
    REQUIRE_THROWS_AS(bencode::parse("i12a3e"), std::invalid_argument);
}

TEST_CASE("Parse integer with plus sign") {
    REQUIRE_THROWS_AS(bencode::parse("i+3e"), std::invalid_argument);
}

TEST_CASE("Parse integer with whitespace") {
    REQUIRE_THROWS_AS(bencode::parse("i 3e"), std::invalid_argument);
}

TEST_CASE("Parse integer negative leading zero") {
    REQUIRE_THROWS_AS(bencode::parse("i-0123e"), std::invalid_argument);
}

TEST_CASE("Parse integer double minus") {
    REQUIRE_THROWS_AS(bencode::parse("i--1e"), std::invalid_argument);
}

TEST_CASE("Parse integer missing digits after minus") {
    REQUIRE_THROWS_AS(bencode::parse("i-e"), std::invalid_argument);
}

//////////////////
// String tests
//////////////////
TEST_CASE("Parse string 1") {
    auto res = bencode::parse("4:spam");
    REQUIRE(std::holds_alternative<std::string>(res.value));
    REQUIRE(std::get<std::string>(res.value) == "spam");
}

TEST_CASE("Parse string 2") {
    auto res = bencode::parse("3:abc");
    REQUIRE(std::holds_alternative<std::string>(res.value));
    REQUIRE(std::get<std::string>(res.value) == "abc");
}

TEST_CASE("Parse very long string") {
    std::string longStr(10000, 'a');
    std::string bencoded = std::to_string(longStr.size()) + ":" + longStr;
    auto res = bencode::parse(bencoded);
    REQUIRE(std::holds_alternative<std::string>(res.value));
    REQUIRE(std::get<std::string>(res.value) == longStr);
}

TEST_CASE("Parse empty string") {
    auto res = bencode::parse("0:");
    REQUIRE(std::holds_alternative<std::string>(res.value));
    REQUIRE(std::get<std::string>(res.value).empty());
}

TEST_CASE("Parse string with colon content") {
    auto res = bencode::parse("7:foo:bar");
    REQUIRE(std::holds_alternative<std::string>(res.value));
    REQUIRE(std::get<std::string>(res.value) == "foo:bar");
}

TEST_CASE("Parse string with multi-digit length") {
    auto res = bencode::parse("11:hello world");
    REQUIRE(std::holds_alternative<std::string>(res.value));
    REQUIRE(std::get<std::string>(res.value) == "hello world");
}

TEST_CASE("Parse string missing colon") {
    REQUIRE_THROWS_AS(bencode::parse("4spam"), std::invalid_argument);
}

TEST_CASE("Parse string length exceeds data") {
    REQUIRE_THROWS_AS(bencode::parse("5:spam"), std::invalid_argument);
}

TEST_CASE("Parse string non-digit length") {
    REQUIRE_THROWS_AS(bencode::parse("x:spam"), std::invalid_argument);
}

TEST_CASE("Parse string negative length") {
    REQUIRE_THROWS_AS(bencode::parse("-1:spam"), std::invalid_argument);
}

TEST_CASE("Parse string empty length") {
    REQUIRE_THROWS_AS(bencode::parse(":spam"), std::invalid_argument);
}

////////////////
// List tests
////////////////
TEST_CASE("Parse normal list") {
    auto res = bencode::parse("l4:spam3:abci42ee");
    REQUIRE(std::holds_alternative<bencode::List>(res.value));
    const auto& list = std::get<bencode::List>(res.value);
    REQUIRE(list.values.size() == 3);
    REQUIRE(std::get<std::string>(list.values[0]) == "spam");
    REQUIRE(std::get<std::string>(list.values[1]) == "abc");
    REQUIRE(std::get<int64_t>(list.values[2]) == 42);
}

TEST_CASE("Parse empty list") {
    auto res = bencode::parse("le");
    REQUIRE(std::holds_alternative<bencode::List>(res.value));
    const auto& list = std::get<bencode::List>(res.value);
    REQUIRE(list.values.empty());
}

TEST_CASE("Parse list of integers") {
    auto res = bencode::parse("li1ei2ei3ee");
    REQUIRE(std::holds_alternative<bencode::List>(res.value));
    const auto& list = std::get<bencode::List>(res.value);
    REQUIRE(list.values.size() == 3);
    REQUIRE(std::get<int64_t>(list.values[0]) == 1);
    REQUIRE(std::get<int64_t>(list.values[1]) == 2);
    REQUIRE(std::get<int64_t>(list.values[2]) == 3);
}

TEST_CASE("Parse list of mixed types") {
    auto res = bencode::parse("l4:spami-2e0:ee");
    REQUIRE(std::holds_alternative<bencode::List>(res.value));
    const auto& list = std::get<bencode::List>(res.value);
    REQUIRE(list.values.size() == 3);
    REQUIRE(std::get<std::string>(list.values[0]) == "spam");
    REQUIRE(std::get<int64_t>(list.values[1]) == -2);
    REQUIRE(std::get<std::string>(list.values[2]).empty());
}

TEST_CASE("Parse nested list") {
    auto res = bencode::parse("lli1ei2ee4:donee");
    REQUIRE(std::holds_alternative<bencode::List>(res.value));
    const auto& list = std::get<bencode::List>(res.value);
    REQUIRE(list.values.size() == 2);
    REQUIRE(std::holds_alternative<bencode::List>(list.values[0]));
    const auto& inner = std::get<bencode::List>(list.values[0]);
    REQUIRE(inner.values.size() == 2);
    REQUIRE(std::get<int64_t>(inner.values[0]) == 1);
    REQUIRE(std::get<int64_t>(inner.values[1]) == 2);
    REQUIRE(std::get<std::string>(list.values[1]) == "done");
}

TEST_CASE("Parse list containing dict") {
    auto res = bencode::parse("ld3:key5:valueee");
    REQUIRE(std::holds_alternative<bencode::List>(res.value));
    const auto& list = std::get<bencode::List>(res.value);
    REQUIRE(list.values.size() == 1);
    const auto& dict = std::get<bencode::Dict>(list.values[0]);
    REQUIRE(dict.items.size() == 1);
    REQUIRE(dict.items[0].first == "key");
    REQUIRE(std::get<std::string>(dict.items[0].second) == "value");
}

TEST_CASE("Parse list missing terminator") {
    REQUIRE_THROWS_AS(bencode::parse("li1ei2e"), std::invalid_argument);
}

TEST_CASE("Parse list with invalid element") {
    REQUIRE_THROWS_AS(bencode::parse("l4spam3:abce"), std::invalid_argument);
}