#include <Script/UserData/UserData.h>
#include <Script/UserData/Map.h>
#include <Script/UserData/Game.h>
#include <Script/UserData/Unit.h>
#include <Script/UserData/UnitMovement.h>
#include <Script/UserData/Vector2.h>
#include <Script/UserData/AreaDesc.h>
#include <Script/UserData/MovementDescType.h>
#include <Script/UserData/CostTable.h>
#include <Script/UserData/Player.h>

namespace Script::UserData
{
    void Init(lua_State* luaState);
}