# How to get an unit's team ID

The current way to get a team id which a unit belongs to requires to get a Player object through a Game object by using the owner id stored in the Unit object. This creates a dependency
when trying to calculate a unit's path, because the team which the units involved belong to is relevant to the calculation. Tiles which units of the enemy team are placed on have different costs.

## Option 1 - AreaDesc access game

During the calculation, the AreaDesc object has access to the relevant Game object. Then it can perform the current method to access every unit's team ID.

- Requires heavy refactoring. Wherever AreaDesc was used, now a Game instance has to be created.
- Creates a dependency between AreaDesc and Game, when the Game object is only used for accessing players.

## Option 2 - Player pointer/reference in units

Unit objects, instead of storing ids, hold a reference to a Player object. The real object can be held in a Game instance, while a convenient reference is held in every unit which is owned by a given player.

+ Ligth refactoring.
- ~~Can lead to inconsistencies in the game. What if a player is removed?~~ Well, my friend the same would happen with an ID.
- Complicates serialization. Instead of an elegant int, now a reference to a Player object has to be retrieved.

## Option 3 - Units store a cache holding team ID

When a unit is created, they receive a team id which corresponds to the player owner's team. 

+ 0 refactoring
- That variable works a cache, incrases memory use when it should not be necessary.
- Increases Redundancy which can lead to inconsitencies. What if the owner changes team? Or what if the owner's team and the unit's cache var doesn't match.
- Scalability. What if other data in Player becomes relevant?


## Conclusion
We  choose option 2.