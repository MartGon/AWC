#include "doctest.h"

#include <lua.hpp>

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>
#include <Tests/AWC/UnitTest.h>

#include <string>
#include <iostream>

TEST_CASE("Lua Table test")
{
    auto luaState = luaL_newstate();

    // Scope so lt is destroyed before luaState
    {
        Script::LuaTable lt{luaState};

        SUBCASE("Integers")
        {
            lt.SetInt("Int", 1);

            CHECK(lua_gettop(luaState) == 0);

            auto var = lt.GetInt("Int");
            CHECK(var == 1);
        }
        SUBCASE("Strings")
        {
            lt.SetString("String", "str");

            CHECK(lua_gettop(luaState) == 0);

            auto var = lt.GetString("String");
            CHECK(var == "str");
        }

        CHECK(lua_gettop(luaState) == 0);
    }

    lua_close(luaState);
}

TEST_CASE("ScriptType Operations")
{
    Script::Game sg;

    std::string scriptPath = std::string(SCRIPTS_DIR) + "scripttype.lua";
    auto st = sg.CreateScriptType(scriptPath);


#ifdef _DEBUG
    auto luaState = sg.GetLuaState();
    CHECK(lua_gettop(luaState) == 0);

#endif

    SUBCASE("Execution")
    {
        auto script = sg.CreateScript(st);
        auto& table = sg.GetScriptTable(script);
        table.SetInt("value", 3);

        sg.PushScript(script);
        sg.GetGame().Run();

        CHECK(table.GetInt("value") == 6);

        
        sg.PushScript(script);
        sg.GetGame().Run();

        CHECK(table.GetInt("value") == 9);

        sg.PushScript(script);
        CommandPtr null{new NullCommand()};
        sg.GetGame().ExecuteCommand(null);

        CHECK(table.GetInt("value") == 12);
    }
    SUBCASE("Creation with wrong path")
    {
        std::string wrongPath = std::string(SCRIPTS_DIR) + "wrongPath.lua";
        CHECK_THROWS_AS(sg.CreateScriptType(wrongPath), const AWCException&);

#ifdef _DEBUG
        CHECK(lua_gettop(luaState) == 0);
#endif
    }
}

TEST_CASE("Error handling")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();

    Map map{5, 5};
    TileType grassType{0, "Grass"};
    MapUtils::FillMap(map, grassType);

    Player p{0, 0, 1000};
    game.AddPlayer(p);

    auto soldierType = UnitTest::CreateSoldierType();
    auto soldier = soldierType.CreateUnit(p);

    game.AddMap(map);

    game.AddUnit(soldier, {2, 0}, 0);

    SUBCASE("Error handling")
    {
        std::string path = std::string(SCRIPTS_DIR) + "/ErrorHandling.lua";
        Test::Script::TestScript t(path, sGame);
        
        auto L = sGame.GetLuaState();

        auto& sTable = t.lt();
        auto unitMove = soldier->CalculateMovement(game.GetMap(0), {2, 0});
        
        sTable.SetInt("mapIndex", 0);
        sTable.SetFullUserData("origin", Script::UserData::Vector2::MT_NAME, Vector2{0, 0});
        sTable.SetFullUserData("dest", Script::UserData::Vector2::MT_NAME, Vector2{0, 0});

        try{
            sGame.PushScript(t.ref);
            game.Run();
        }
        catch(const AWCException& e)
        {
            std::cout << e.what() << '\n';
        }

        auto s2 = sGame.CreateScript(t.sType);
        auto& s2t = sGame.GetScriptTable(s2);

        s2t.SetInt("mapIndex", 0);
        s2t.SetFullUserData("origin", Script::UserData::Vector2::MT_NAME, Vector2{2, 0});
        s2t.SetFullUserData("dest", Script::UserData::Vector2::MT_NAME, Vector2{0, 0});

        using namespace Script;

        auto origin1 = sTable.GetUserData<::Vector2>("origin", UserData::Vector2::MT_NAME);
        auto origin2 = s2t.GetUserData<::Vector2>("origin", UserData::Vector2::MT_NAME);

        CHECK(*origin1 != * origin2);

        sGame.PushScript(s2);
        game.Run();

        sGame.PushScript(t.ref);
        game.Run();
    }
}
