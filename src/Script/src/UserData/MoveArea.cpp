#include <Script/UserData/MoveArea.h>

#include <Script/UserData/UserData.h>
#include <Script/UserData/Vector2.h>

using namespace Script;

const char* UserData::MoveArea::MT_NAME = "AWC_MoveArea";
const char* UserData::MoveArea::LIB_NAME = "MoveArea";
const luaL_Reg UserData::MoveArea::methods[] = {
    {"CanMove", MoveArea::CanMove},
    {"GetTilesPosInReachCount", MoveArea::GetTilesPosInReachCount},
    {"GetTilePosInReach", MoveArea::GetTilePosInReach},
    {NULL, NULL}
};
const luaL_Reg UserData::MoveArea::functions[] = {
    {NULL, NULL}
};

int UserData::MoveArea::CanMove(lua_State* luaState)
{
    auto unitMove = UserData::CheckUserData<MoveArea>(luaState, 1);
    auto dest = UserData::CheckUserData<Vector2>(luaState, 2);

    bool result = unitMove->CanMove(*dest);
    lua_pushboolean(luaState, result);

    return 1;
}

int UserData::MoveArea::GetTilesPosInReachCount(lua_State* luaState)
{
    auto unitMove = UserData::CheckUserData<MoveArea>(luaState, 1);
    auto count = unitMove->GetTilesPosInReach().size();

    lua_pushinteger(luaState, count);

    return 1;
}

int UserData::MoveArea::GetTilePosInReach(lua_State* luaState)
{
    auto unitMove = UserData::CheckUserData<MoveArea>(luaState, 1);
    auto index = luaL_checkinteger(luaState, 2) - 1;

    auto tilesPos = unitMove->GetTilesPosInReach();
    auto count = tilesPos.size();
    bool validIndex = index < count && index >= 0;
    CheckArg<Scope::Internal>(luaState, validIndex, 2, "Pos index " + std::to_string(index) + " is not valid");

    UserData::PushDataCopy<Vector2>(luaState, tilesPos[index]);

    return 1;
}