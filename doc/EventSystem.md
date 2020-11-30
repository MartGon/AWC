# EventSystem

## Option 1: Event Queue

## Option 2: Observer

## Option 3: ???

## Notes

Events triggered by commands could be held within the command itself. The event listener actors should handle events and interact with the game state with a set of well defined operations, which upon being called are recorded in the command which triggered it. This should be useful when a given command has to be undone. E.g. 

User executes an AttackCommand which results in a Unit death that has an OnDeath effect, which triggers an explosion and damages nearby enemy units.

1. The unit dies and triggers an OnDeath event
2. The unit itself handles the event. It scans nearby units.
3. Applies a damage effect to every unit around via a well-defined operation
4. This is recorded within the AttackCommand.
