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
}