# TODO

- Operations don't need to have ids, process should be the ones with id. This way, the factory is no longer needed - DONE
- Operation Type should be an unsigned int instead of an enum class. This way it's extendable, while some constants are still defined - DONE
- With LuaTable class, beware of destructor call order. Atm ScriptGame is called before Game destructor, so Scripts on history are cleared right after the luaState has been close! - DONE. A LuaState wrapper has been created to avoid this issue
- Move code to a scripting library
- Game Lightuserdata
- Map Lightuserdata
- Unit Lightuserdata
- UnitMovement Lightuserdata


# Ideas

- Add compenent array to Game. Components are a base class which hold just a type and a GetType method. This way, the functionality of game can be extended without game the specific type. E. g: A script component could be created, which holds the luaState. This avoids type casting the game. Game could have an interface to query these components by type or Id.

This has been avoided because Script Operation are managed by ScriptGame. However, this design could still be useful for other purposes, such as extending functionality in a general way and specially for EventHandler.