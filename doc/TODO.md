# TODO

- Operations don't need to have ids, process should be the ones with id. This way, the factory is no longer needed - DONE
- Operation Type should be an unsigned int instead of an enum class. This way it's extendable, while some constants are still defined - DONE
- With LuaTable class, beware of destructor call order. Atm ScriptGame is called before Game destructor, so Scripts on history are cleared right after the luaState has been close! - DONE. A LuaState wrapper has been created to avoid this issue
- Move code to a scripting library - DONE
- Game Lightuserdata - DONE
- Map Lightuserdata - DONE
- Unit Lightuserdata - DONE
- UnitMovement Lightuserdata - DONE


# Ideas

- Add compenent array to Game. Components are a base class which hold just a type and a GetType method. This way, the functionality of game can be extended without game the specific type. E. g: A script component could be created, which holds the luaState. This avoids type casting the game. Game could have an interface to query these components by type or Id.
- Lua Configuration: Provide a way to add operations, units, tiles from Lua. This file could be called "configuration.lua" or something similar and it would be loaded from the `Script::Game` object with a `LoadConfigurationFile(std::string path);` method. The lua methos to add stuff could be similar to
```
  game.addOperation({
    name = "Move"
    execute = function(game)
      ...
    end
  }
  
  game.addUnit({
    name = "Soldier"
    moveType = MoveType.New({
      ...
    })
  })
```

This has been avoided because Script Operation are managed by ScriptGame. However, this design could still be useful for other purposes, such as extending functionality in a general way and specially for EventHandler.
