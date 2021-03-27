#pragma once

#include <lua.hpp>

#include <string>
#include <type_traits>
#include <functional>

namespace Script
{
    enum class Scope
    {
        Internal,
        External
    };

    template<typename T>
    void Push(lua_State* state, T val)
    {
        val.PushInternal();
    }

    template<>
    void Push<int>(lua_State* state, int val);
    template<>
    void Push<std::string>(lua_State* state, std::string val);
    template<>
    void Push<const char*>(lua_State* state, const char* val);
    template<>
    void Push<bool>(lua_State* state, bool val);

    template <typename T>
    T To(lua_State* state, int index)
    {
        return T{state, index};
    }

    template<>
    int To<int>(lua_State* state, int index);
    template<>
    unsigned int To<unsigned int>(lua_State* state, int index);
    template<>
    std::string To<std::string>(lua_State* state, int index);
    template<>
    bool To<bool>(lua_State* state, int index);

    template <typename T>
    int GetField(lua_State*, int index, T key);
    template<>
    int GetField<int>(lua_State*, int index, int key);
    template<>
    int GetField<const char*>(lua_State*, int index, const char* key);
    template<>
    int GetField<std::string>(lua_State*, int index, std::string key);

    bool IsType(lua_State* L, int index, int type);
    bool IsTable(lua_State*, int index);
    bool IsFunction(lua_State*, int index);

    template<typename K>
    std::enable_if_t<std::is_integral_v<K>, std::string> KeyToString(K key)
    {
        return std::to_string(key);
    }
    template<typename K>
    std::enable_if_t<std::is_pointer_v<K>, std::string> KeyToString(K key)
    {
        return std::string(key);
    }
    template<typename K>
    std::enable_if_t<std::is_same_v<K, std::string>, std::string> KeyToString(K key)
    {
        return key;
    }


    template<Scope>
    void Check(lua_State* luaState, bool condition, std::string msg, 
        std::function<void(lua_State*)> errHandler = [](lua_State*){});
    template<>
    void Check<Scope::Internal>(lua_State* luaState, bool condition, std::string msg, 
        std::function<void(lua_State*)> errHandler);
    template<>
    void Check<Scope::External>(lua_State* luaState, bool condition, std::string msg, 
        std::function<void(lua_State*)> errHandler);

    template<Scope>
    void CheckArg(lua_State* luaState, bool condition, int index, std::string msg, 
        std::function<void(lua_State*)> errHandler = [](lua_State*){});
    template<>
    void CheckArg<Scope::Internal>(lua_State* luaState, bool condition, int index, std::string msg, 
        std::function<void(lua_State*)> errHandler);
    template<>
    void CheckArg<Scope::External>(lua_State* luaState, bool condition, int index, std::string msg, 
        std::function<void(lua_State*)> errHandler);

    template<Scope>
    void CheckExpectedArg(lua_State* luaState, bool condition, int index, std::string type,
        std::function<void(lua_State*)> errHandler = [](lua_State*){});
    template<>
    void CheckExpectedArg<Scope::Internal>(lua_State* luaState, bool condition, int index, std::string type,
        std::function<void(lua_State*)> errHandler);
    template<>
    void CheckExpectedArg<Scope::External>(lua_State* luaState, bool condition, int index, std::string type,
        std::function<void(lua_State*)> errHandler);

    template<Scope s, typename K>
    void CheckArgInTable(lua_State* luaState, bool condition, int sIndex, K tIndex, std::string msg,
        std::function<void(lua_State*)> errHandler = [](lua_State*){})
    {
        std::string err = "Table[" + KeyToString(tIndex) + "]: " + msg;
        CheckArg<s>(luaState, condition, sIndex, err, errHandler);
    }

    template<Scope s, typename K>
    void CheckExpectedArgInTable(lua_State* luaState, bool condition, int sIndex, K tIndex, std::string type,
        std::function<void(lua_State*)> errHandler = [](lua_State*){})
    {
        std::string err = "Table[" + KeyToString(tIndex) + "]: " + type;
        CheckExpectedArg<s>(luaState, condition, sIndex, err, errHandler);
    }
}