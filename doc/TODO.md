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
- OperationFactory which holds the next operation id
- CounterOperation: Operation which removes another operation from the queue
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
    They hold a reference to event/Subject, so the map/player interface can be used directly. No need to make proxy methods in game (Add/RemoveUnit)

## GUID vs Entity

## Idea 1 - Subject holds GUIDs

- Callbacks are passed a Notifcation, GUID and Game&
- Callbacks need to find their entity (Unit, Tile, Effect, etc.) by using a GUID. 
    Problem: GUIDs are not looked up uniformely by Game, e.g. Unit and Effect (when added)
- Simple Callbacks have a NIL/DUMMY GUID type, with subtype and id ignored

## Idea 2 - Subject holds Entities

- Entities have the following interface

Entity - Anything that listens to events and may change the state of the game
  GUID GetGUID()
  virtual void RegisterHandlers(Subject& subject);
  T To<T>();

- Unit, Tile, Effect, etc. inherit from Entity
- Callbacks receive an Entity, instead of a GUID. They need to cast it to the appropiate type, though

Problem: RegisterHandlers cannot pass a shared_ptr to itself to create a Event::Listener
Possible Solutions:
  1. Entity inherits from std::enable_shared_from_this<A> and a call to std::shared_from_this() is made
    Cons: It isn't a very clean solution. I'm not happy with it
  2. Listener holds a reference to Entity
    Cons: A invalid ref could be hold if the object is deleted. It shouldn't happen tho, the entity should remove the listeners first when it is removed
  3. Listener holds a raw pointer to Entity
    Cons: The same as 2, but with it would also be less readable imo
  4. Instead of RegisterHandlers, there's a GetHandlers methods which returns the handlers. The calling methods should create the Listeners and call Register;
    Cons: This is error prone
  