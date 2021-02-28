#include <AWC.h>

#include <lua.hpp>

namespace Script
{
    namespace UserData
    {
        namespace Game
        {
            void Init(lua_State* L);
            void Push(lua_State* L, ::Game* game);

            int GetMap(lua_State* L);
            int GetMapCount(lua_State* L);
        }
    }
}