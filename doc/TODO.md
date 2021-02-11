# TODO

## Event System

- Create Operation Queue in Game - Done
- Run method in Game that executes current operations - Done
- Adapt commands to use Operations
    - MoveCommand - Done
    - AttackCommand - Done
- Register Observers - 1/2 Done
- Game::AddUnit methods which registers handlers
- Game::RemoveUnit methods which removes its handlers
- Subject::Register by notification type - DONE
- Subject::Remove - Done
- Subject use a while loop instead of for - Undone -> Can cause infinite loops and undesired behaviour in general. It's also simpler the other way.
- UnitTests for each of the methods
- Functional test - Hugo and Large tank
- Tile and Unit inherit from Entity. Then create GUID{EntityType, SubType, id}. Use GUID for Subject

## Spare ideas

- Unit Graveyard: Dead units go to the graveyard, they can still listen to events. Example: Magic's phoenix
- Null Entity: This allows to use the Event system without using an entity.
- Re organize stuff in proper namespaces. They're polluting global scope
- Re organize headers. Much stuff is being imported while not being needed.
- Use std::enable_shared_prt_to_this;
- Do something about exceptions
- Players/Maps can only be created from Game:
    They hold a reference to event, so the map/player interface can be used directly. No need to make proxy methods in game