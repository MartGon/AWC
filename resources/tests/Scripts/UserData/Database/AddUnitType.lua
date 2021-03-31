

local vec1 = Vector2.new(0, 1);
local vec2 = Vector2.new(1, 0);
local tp = AreaDesc.New({
    
    directions = {
        vec1,
        vec2
    }
});
local mdt = MovementDescType.New({
    tpd = tp,
    range = {
        min = 1,
        max = -1;
    },
    tileCT = {
        entries = {
            {id = 1, cost = 1}
        },
        default = 1
    },
    unitCT = {
        entries = {
            {id = 1, cost = 1}
        },
        default = 1
    },
    maxGas = -1;
});
local weaponType = WeaponType.New({
    tpd = tp,
    range = {
        min = 1,
        max = -1;
    },
    attackTable = {
        {id = 1, attackable = true}
    },
    dmgTable = {
        {id = 1, dmg = 25.5}
    }
})
local eventHandler = { opType = 1, callback = function ()
    i = 2 + 3;
end}

function Execute(game)

    local db = DB;
    print(db);
    db:AddUnitType({
        name = "Rook",
        moveType = mdt;
        weapons = {
            weaponType
        },
        eventHandlers = {
            eventHandler
        }
    });
    print("hey");

end

function Undo(game)

end