#include <Script/UserData/UnitMovement.h>

#include <Script/UserData/UserData.h>
#include <Script/UserData/Vector2.h>

using namespace Script;

const char* UserData::UnitMovement::MT_NAME = "AWC_UnitMovement";
const char* UserData::UnitMovement::LIB_NAME = "UnitMovement";
const luaL_Reg UserData::UnitMovement::methods[] = {
    {"CanMove", UnitMovement::CanMove},
    {"GetTilesPosInReachCount", UnitMovement::GetTilesPosInReachCount},
    {"GetTilePosInReach", UnitMovement::GetTilePosInReach},
    {NULL, NULL}
};
const luaL_Reg UserData::UnitMovement::functions[] = {
    {NULL, NULL}
};

int UserData::UnitMovement::CanMove(lua_State* luaState)
{
    auto unitMove = UserData::CheckUserData<UnitMovement>(luaState, 1);
    auto dest = UserData::CheckUserData<Vector2>(luaState, 2);

    bool result = unitMove->CanMove(*dest);
    lua_pushboolean(luaState, result);

    return 1;
}

int UserData::UnitMovement::GetTilesPosInReachCount(lua_State* luaState)
{
    auto unitMove = UserData::CheckUserData<UnitMovement>(luaState, 1);
    auto count = unitMove->GetTilesPosInReach().size();

    lua_pushinteger(luaState, count);

    return 1;
}

int UserData::UnitMovement::GetTilePosInReach(lua_State* luaState)
{
    auto unitMove = UserData::CheckUserData<UnitMovement>(luaState, 1);
    auto index = luaL_checkinteger(luaState, 2) - 1;

    auto tilesPos = unitMove->GetTilesPosInReach();
    auto count = tilesPos.size();
    bool validIndex = index < count && index >= 0;
    CheckArg<Scope::Internal>(luaState, validIndex, 2, "Pos index " + std::to_string(index) + " is not valid");

    UserData::PushDataCopy<Vector2>(luaState, tilesPos[index]);

    return 1;
}