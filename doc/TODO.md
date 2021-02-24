# TODO

- Operations don't need to have ids, process should be the ones with id. This way, the factory is no longer needed
- Add compenent array to Game. Components are a base class which hold just a type and a GetType method. This way, the functionality of game can be extended without game the specific type. E. g: A script component could be created, which holds the luaState. This avoids type casting the game. Game could have an interface to query these components by type or Id.
- With LuaTable class, beware of destructor call order. Atm ScriptGame is called before Game destructor, so Scripts on history are cleared right after the luaState has been close!