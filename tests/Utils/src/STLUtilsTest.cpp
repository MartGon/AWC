#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <Utils.h>

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
    SUBCASE("Slice")
    {
        std::vector<int> a{1, 2, 3, 4};
        std::vector<int> expectedResult1{3, 4};
        std::vector<int> expectedResult2{2, 3, 4};
        std::vector<int> expectedResult3{1, 2, 3, 4};
        std::vector<int> expectedResult4{2, 3};

        auto result1 = VectorUtils::Slice(a, 2, a.size() - 2);
        auto result2 = VectorUtils::Slice(a, 1, a.size() - 1);
        auto result3 = VectorUtils::Slice(a, 0, a.size());
        auto result4 = VectorUtils::Slice(a, 1, 2);

        CHECK(result1 == expectedResult1);
        CHECK(result2 == expectedResult2);
        CHECK(result3 == expectedResult3);
        CHECK(result4 == expectedResult4);

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
    SUBCASE("Split")
    {
        std::string source = "Is the end of the world";
        char delim = ' ';
        std::vector<std::string> expectedResult = {"Is", "the", "end", "of", "the", "world"};
        std::vector<std::string> expectedResult2 = {"Is the end o", " the world"};

        CHECK(expectedResult == StringUtils::Split(source, delim));
        CHECK(expectedResult2 == StringUtils::Split(source, 'f'));
    }
}

TEST_CASE("Unordered map test")
{
    std::unordered_map<int, std::string> map{{1, "Hola"}, {2, "Adios"}};
    SUBCASE("Contains")
    {
        CHECK(UnorderedMapUtils::Contains(map, 1) == true);
        CHECK(UnorderedMapUtils::Contains(map, 2) == true);

        CHECK(UnorderedMapUtils::Contains(map, 3) == false);
        CHECK(UnorderedMapUtils::Contains(map, 4) == false);
    }
}