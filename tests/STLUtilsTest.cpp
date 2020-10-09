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
}