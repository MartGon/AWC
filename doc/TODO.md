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
- Subject::Add/Remove
- Subject use a while loop instead of for
- UnitTests for each of the methods
- Functional test - Hugo and Large tank

## Spare ideas

- Unit Graveyard: Dead units go to the graveyard, they can still listen to events. Example: Magic's phoenix
- Null Entity: This allows to use the Event system without using an entity.
- 