# Desing Decisions

## What can be scripted ?

- Game operations
- Event handlers
- TilePatternDescriptors
- Win Conditions

## How can they be scripted ?

### 1. Game Operations

Every Game operation has the following interface:

```

class Operation
{
public:
    void Execute(Game& game);
    void Undo(Game& game);

    // Data
}
```

So an operation implemented in Lua could be defined in several ways

1. A Lua function implemented in C++ could be created for this purpose, with a signature like the next one.

```
CreateOperation(execute, undo); -- Creates a new type of Operation
CreateOperationInstance(id, data); -- Creates an instance of a given OP
```

2. In a Lua file. However this has a limitation, only one operation can be declared per file

```
function Execute(game)
-- Code
end

function Undo(game)
-- Code
end

var1 = 'A';
var2 = 1;
...
```

**Full User Data vs Light User Data**

Operation's lifetime is managed by AWC, as they are held in the Operation History, thus Light User Data is the only realistic option.


### Event Handlers

Events handlers can be registered by different means:

1. By calling a function

```
RegisterHandler(opType, handlerCallback, notificationType);
```

2. By declaring the function in a Lua file

```
// eventHandler.json

[
    ...,
    {
        "opType" : "MOVE",
        "notType" : "POST",
        "handler" : "moveHandler.lua"
    }
]
```

## Script loading structure

Create lua files which are aimed to create instances of scriptable entities:

1. Operations.lua file. This file contains the custom Script operations
