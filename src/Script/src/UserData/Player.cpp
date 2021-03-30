#include <Script/UserData/Player.h>
#include <Script/UserData/UserData.h>

using namespace Script;

const char* UserData::Player::MT_NAME = "AWC_Player";
const char* UserData::Player::LIB_NAME = "Player";

const luaL_Reg UserData::Player::methods[] = {
    {"GetTeamId", Player::GetTeamId},
    {"GetId", Player::GetId},
    {NULL, NULL}
};

const luaL_Reg UserData::Player::functions[] = {
    {NULL, NULL}
};

int UserData::Player::GetTeamId(lua_State* luaState)
{
    auto player = UserData::CheckUserData<Player>(luaState, 1);
    lua_pushinteger(luaState, player->GetTeamId());

    return 1;
}

int UserData::Player::GetId(lua_State* luaState)
{
    auto player = UserData::CheckUserData<Player>(luaState, 1);
    lua_pushinteger(luaState, player->GetId());

    return 1;
}