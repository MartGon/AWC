#pragma once

#include <lua.hpp>

#include <string>

namespace Script
{
    template<typename T>
    void Push(lua_State* state, T val);

    template<>
    void Push<int>(lua_State* state, int val);
    template<>
    void Push<std::string>(lua_State* state, std::string val);
    template<>
    void Push<const char*>(lua_State* state, const char* val);
    template<>
    void Push<bool>(lua_State* state, bool val);

    template <typename T>
    T To(lua_State* state, int index);
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
}