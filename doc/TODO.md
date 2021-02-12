# TODO

## Event System

- Create Operation Queue in Game - DONE
- Run method in Game that executes current operations - DONE
- Adapt commands to use Operations
    - MoveCommand - DONE
    - AttackCommand - DONE
- Register Observers - 1/2 DONE
- Game::AddUnit methods which registers handlers - DONE
- Game::RemoveUnit methods which removes its handlers - DONE
- Subject::Register by notification type - DONE
- Subject::Remove - DONE
- Subject use a while loop instead of for - UNDONE -> Can cause infinite loops and undesired behaviour in general. It's also simpler the other way.
- UnitTests for each of the methods - DONE
- Functional test - Hugo and Large tank
- Tile and Unit inherit from Entity. Then create GUID{EntityType, SubType, id}. Use GUID for Subject. Callbacks have dummy entities. Entity has a GetGUID method.
  It could also have a To<> method which asserts if it's a entity's son

## Spare ideas

- Unit Graveyard: Dead units go to the graveyard, they can still listen to events. Example: Magic's phoenix
- Null Entity: This allows to use the Event system without using an entity.
- Re organize stuff in proper namespaces. They're polluting global scope
- Re organize headers. Much stuff is being imported while not being needed.
- Use std::enable_shared_prt_to_this;
- Do something about exceptions
- Players/Maps can only be created from Game:
    They hold a reference to event, so the map/player interface can be used directly. No need to make proxy methods in game