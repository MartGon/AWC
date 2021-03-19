#include "doctest.h"

#include <lua.hpp>

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>
#include <Tests/AWC/UnitTest.h>

#include <string>
#include <iostream>

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
        table.Set("value", 3);

        sg.PushScript(script);
        sg.GetGame().Run();

        CHECK(table.Get<int>("value") == 6);
        
        sg.PushScript(script);
        sg.GetGame().Run();

        CHECK(table.Get<int>("value") == 9);

        sg.PushScript(script);
        CommandPtr null{new NullCommand()};
        sg.GetGame().ExecuteCommand(null);

        CHECK(table.Get<int>("value") == 12);
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
        
        sTable.Set("mapIndex", 0);
        sTable.SetDataCopy<Script::UserData::Vector2>("origin", Vector2{0, 0});
        sTable.SetDataCopy<Script::UserData::Vector2>("dest", Vector2{0, 0});

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

        s2t.Set("mapIndex", 0);
        s2t.SetDataCopy<Script::UserData::Vector2>("origin", Vector2{2, 0});
        s2t.SetDataCopy<Script::UserData::Vector2>("dest", Vector2{0, 0});

        using namespace Script;

        auto origin1 = sTable.GetUserData<Script::UserData::Vector2>("origin");
        auto origin2 = s2t.GetUserData<Script::UserData::Vector2>("origin");

        CHECK(*origin1 != * origin2);

        sGame.PushScript(s2);
        game.Run();

        sGame.PushScript(t.ref);
        game.Run();
    }
}
