
#include <AWC.h>

#include <Script/ScriptOperation.h>
#include <Script/Wrappers/LuaFunction.h>

namespace Script
{
    class LuaCallback : public Event::CallbackI
    {
    public:
        LuaCallback(lua_State* L, int state) : L{L}, f{L, state}
        {

        }

        void Call(const Event::Notification::Notification, Entity::GUID, ::Game&);

    private:
        lua_State* L;
        LuaFunction<Scope::External> f;
    };
}