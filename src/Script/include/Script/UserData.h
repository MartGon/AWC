#include <Script/UserData/UserData.h>
#include <Script/UserData/Map.h>
#include <Script/UserData/Game.h>
#include <Script/UserData/Unit.h>
#include <Script/UserData/MoveArea.h>
#include <Script/UserData/Vector2.h>
#include <Script/UserData/AreaDesc.h>
#include <Script/UserData/MovementDescType.h>
#include <Script/UserData/CostTable.h>
#include <Script/UserData/Player.h>
#include <Script/UserData/UnitType.h>
#include <Script/UserData/TileType.h>
#include <Script/UserData/Tile.h>
#include <Script/UserData/Process.h>
#include <Script/UserData/Trigger.h>
#include <Script/UserData/WeaponType.h>
#include <Script/UserData/AttackArea.h>
#include <Script/UserData/EventHandler.h>
#include <Script/UserData/EventNotification.h>
#include <Script/UserData/EntityGUID.h>
#include <Script/UserData/Operation.h>

namespace Script::UserData
{
    void Init(lua_State* luaState);
}