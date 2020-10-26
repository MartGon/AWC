#include "doctest.h"

#include <Utils/STLUtils.h>

// Size checking
TEST_CASE("Vector test")
{
    std::vector<int> vec = {1, 2, 3, 4, 5};

    SUBCASE("Removal")
    {
        VectorUtils::RemoveByValue(vec, 4);
        std::vector<int> result = {1, 2, 3, 5};

        CHECK(result == vec);
        CHECK(result != std::vector<int>{1, 2});
    }
    SUBCASE("IsInside")
    {
        CHECK(VectorUtils::IsInside(vec, 1) == true);
        CHECK(VectorUtils::IsInside(vec, 2) == true);
        CHECK(VectorUtils::IsInside(vec, 3) == true);
        CHECK(VectorUtils::IsInside(vec, 4) == true);
        CHECK(VectorUtils::IsInside(vec, 5) == true);

        CHECK(VectorUtils::IsInside(vec, 6) == false);
        CHECK(VectorUtils::IsInside(vec, 7) == false);
        CHECK(VectorUtils::IsInside(vec, 8) == false);
        CHECK(VectorUtils::IsInside(vec, 9) == false);

        vec.push_back(8);
        vec.push_back(6);

        CHECK(VectorUtils::IsInside(vec, 6) == true);
        CHECK(VectorUtils::IsInside(vec, 8) == true);
    }
    SUBCASE("Union")
    {
        std::vector<int> b{1, 4, 5, 6, 7};
        auto res = VectorUtils::Union(vec, b);

        CHECK(VectorUtils::IsInside(res, 1) == true);
        CHECK(VectorUtils::IsInside(res, 2) == true);
        CHECK(VectorUtils::IsInside(res, 3) == true);
        CHECK(VectorUtils::IsInside(res, 4) == true);
        CHECK(VectorUtils::IsInside(res, 5) == true);
        CHECK(VectorUtils::IsInside(res, 6) == true);
        CHECK(VectorUtils::IsInside(res, 7) == true);

        CHECK(VectorUtils::IsInside(res, 8) == false);
        CHECK(VectorUtils::IsInside(res, 9) == false);
    }
    SUBCASE("Diff")
    {
        std::vector<int> right{1, 3, 5};
        std::vector<int> expectedResult{2, 4};

        auto res = VectorUtils::Diff(vec, right);

        CHECK(VectorUtils::IsInside(res, 2) == true);
        CHECK(VectorUtils::IsInside(res, 4) == true);
        CHECK(VectorUtils::IsInside(res, 1) == false);
        CHECK(VectorUtils::IsInside(res, 3) == false);
        CHECK(VectorUtils::IsInside(res, 5) == false);

        CHECK(res == expectedResult);
    }
    SUBCASE("Intersect")
    {
        std::vector<int> b{-2, -1, 1, 3, 5, 7};
        std::vector<int> expectedResult{1, 3, 5};

        auto res = VectorUtils::Intersect(vec, b);

        CHECK(VectorUtils::IsInside(res, 1) == true);
        CHECK(VectorUtils::IsInside(res, 3) == true);
        CHECK(VectorUtils::IsInside(res, 5) == true);
        CHECK(VectorUtils::IsInside(res, 2) == false);
        CHECK(VectorUtils::IsInside(res, 7) == false);

        CHECK(res == expectedResult);
    }
}

TEST_CASE("String test")
{
    std::string string = "A quick fox jumps over a rock\n";

    SUBCASE("Replace")
    {
        std::string tokenA = "fox";
        std::string replacerA = "toad";
        const std::string expectedResultA = "A quick toad jumps over a rock\n";

        std::string resA = StringUtils::Replace(string, tokenA, replacerA);
        CHECK(resA == expectedResultA);

        std::string tokenB = "rock";
        std::string replacerB = "tree";
        const std::string expectedResultB = "A quick fox jumps over a tree\n";

        std::string resB = StringUtils::Replace(string, tokenB, replacerB);
        CHECK(resB == expectedResultB);
    }
}