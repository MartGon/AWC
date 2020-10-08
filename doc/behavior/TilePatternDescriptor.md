# TilePatternDescriptor

Represents how a **Unit** moves/attacks ignoring the state of the **Map**.

## Responsibility

The main purpose of this class is to provide a flexible way to describe the movement or attacks of a given **Unit**, regardless of the state of the **Map** that unit is in. E.g. This class can be used to represent the chess **Rook** movement in Chess. It moves in a cross pattern, however this is just theoretical. The **Rook** can't move through other units, thus the **TilePattern**, which describes the legal possible movements the **Rook** can take, is different depending on the state of the **Map**.

## Features

This class provides the following functionality

- Give a set of directions the unit can move to. E.g. A **Rook** Chess piece can move in the following directions: **{(1, 0), (-1, 0), (0, 1), (0, -1)}**, which translates to: East, West, North and South.
- Allows to specify a set of *locked* directions once a specific direction has been previously taken. E.g. Once a **Rook** moves north, its direction is *locked* to north.
- Supports creating pattern by composition. E.g. This allows to describe the movement of a **Queen** by combining the movement of a **Rook** and a **Bishop**
- Create a **TilePattern**, which the describes the possible movements/attack given certain **TilePatternConstraints**.

## Design decisions

### Range

The number of tiles a unit can move isn't always unlimited. The **King** and the **Pawn** moves have specific **range**.

**As method parameter**

- When given as a param to one of the methods to create a **TilePattern**, it increases the number of scenarios when an instance of this class can be used. E.g. Chess **King** and **Queen** have the same movement pattern, however the **King** range is limited to one tile.

- It supports movement changes. If the range was tied to this class, a new instance would have to be created each time the range of a unit changed. This is something that can occur frequently in games like AW, where the movement range of a unit can change under certain coditions, such as weather or power ups.

- It hinders composition. What if the range of a pattern is always the same regardless of the context and it should never be altered? More importantly, what if just one member of a composition needs this condition to hold?

    

