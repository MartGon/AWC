#include <doctest.h>

#include <AWC/UnitType.h>
#include <AWC/Unit.h>

// Size checking
TEST_CASE("Units have some properties")
{
    std::string unitName = "Soldier";
    UnitType soldierUnitType{unitName};

    SUBCASE("Units are of a given type")
    {
        auto soldier = soldierUnitType.CreateUnit();
        CHECK(soldier->GetName() == unitName);
    }
}