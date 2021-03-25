#include "doctest.h"

#include <AWC.h>
#include <Script.h>

using namespace Script;

TEST_CASE("ToString")
{
    CHECK(KeyToString(1) == "1");
    CHECK(KeyToString("Hey") == "Hey");
    std::string str{"Test"};
    CHECK(KeyToString(str) == "Test");
}