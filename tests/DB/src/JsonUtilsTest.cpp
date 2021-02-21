#include "doctest.h"
#include "json.hpp"

#include <DB/AWCSer.h>
#include <DB/TilePatternDescSer.h>
#include <DB/JsonUtils.h>

using Json = nlohmann::json;

TEST_CASE("JsonUtils GetValueUnsafe")
{
    Json json{
                {"int", 1}, {"float", 1.5f}, {"std::string", "string"}, {"bool", true},
                {"json", {{"int", 1}, {"float", 1.2f}}}, 
                {"array", {1, 2, 3, 4}}
            };

    Json intObj = json.at("int");
    Json jsonObj = json.at("json");
    Json floatObj = json.at("float");
    Json stringObj = json.at("std::string");
    Json arrayObj = json.at("array");
    Json boolObj = json.at("bool");

    SUBCASE("Get int")
    {
        CHECK_NOTHROW(JsonUtils::GetValueUnsafe<int>(intObj));
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<int>(jsonObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<int>(floatObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<int>(stringObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<int>(arrayObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<int>(boolObj), const Json::type_error&);
    }
    SUBCASE("Get float")
    {
        CHECK_NOTHROW(JsonUtils::GetValueUnsafe<float>(floatObj));
        CHECK_NOTHROW(JsonUtils::GetValueUnsafe<float>(intObj));
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<float>(jsonObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<float>(stringObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<float>(arrayObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<float>(boolObj), const Json::type_error&);
    }
    SUBCASE("Get std::string")
    {
        CHECK_NOTHROW(JsonUtils::GetValueUnsafe<std::string>(stringObj));
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<std::string>(intObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<std::string>(jsonObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<std::string>(floatObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<std::string>(arrayObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<std::string>(boolObj), const Json::type_error&);
    }
    SUBCASE("Get bool")
    {
        CHECK_NOTHROW(JsonUtils::GetValueUnsafe<bool>(boolObj));
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<bool>(intObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<bool>(jsonObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<bool>(floatObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<bool>(arrayObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<bool>(stringObj), const Json::type_error&);
    }
    SUBCASE("Get json")
    {
        CHECK_NOTHROW(JsonUtils::GetValueUnsafe<Json>(jsonObj));
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<Json>(intObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<Json>(boolObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<Json>(floatObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<Json>(arrayObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<Json>(stringObj), const Json::type_error&);
    }
    SUBCASE("Get Array")
    {
        CHECK_NOTHROW(JsonUtils::GetValueUnsafe<Json::array_t>(arrayObj));
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<Json::array_t>(intObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<Json::array_t>(boolObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<Json::array_t>(floatObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<Json::array_t>(jsonObj), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValueUnsafe<Json::array_t>(stringObj), const Json::type_error&);
    }
    SUBCASE("Get misc")
    {
        CHECK_NOTHROW(JsonUtils::GetValueUnsafe<TilePatternDescSer::TilePatternDescType>(intObj));
    }
}

TEST_CASE("JsonUtils GetValue")
{
    Json json{
                {"int", 1}, {"float", 1.5f}, {"std::string", "string"}, {"bool", true},
                {"json", {{"int", 1}, {"float", 1.2f}}}, 
                {"array", {1, 2, 3, 4}}
            };

    Json arrayObj = json.at("array");

    SUBCASE("Get Value by key")
    {
        CHECK_NOTHROW(JsonUtils::GetValue<int>(json, "int"));
        CHECK_THROWS_AS(JsonUtils::GetValue<int>(json, "noint"), const Json::other_error&);
        CHECK_THROWS_AS(JsonUtils::GetValue<int>(json, "std::string"), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValue<int>(json, "std::string", 3), const Json::type_error&);
        CHECK(JsonUtils::GetValue<int>(json, "noint", 3) == 3);
        
    }
    SUBCASE("Get value by index")
    {
        CHECK_NOTHROW(JsonUtils::GetValue<int>(arrayObj, 0));
        CHECK_THROWS_AS(JsonUtils::GetValue<int>(arrayObj, -1), const Json::out_of_range&);
        CHECK_THROWS_AS(JsonUtils::GetValue<std::string>(arrayObj, 0), const Json::type_error&);
        CHECK_THROWS_AS(JsonUtils::GetValue<std::string>(arrayObj, 0, "3"), const Json::type_error&);
        CHECK(JsonUtils::GetValue<int>(json, 4, 3) == 3);
    }
}