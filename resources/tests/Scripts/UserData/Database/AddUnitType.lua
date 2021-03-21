
local function GetMoveType()

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

    return mdt;
end



function Execute(game)

    local db = DB;
    print(db);
    db:AddUnitType({
        name = "Rook",
        moveType = GetMoveType();
    });
    print("hey");

end

function Undo(game)

end