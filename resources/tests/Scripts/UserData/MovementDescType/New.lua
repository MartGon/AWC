

function Execute(game)

    local vec1 = Vector2.new(0, 1);
    local vec2 = Vector2.new(1, 0);
    local tp = TilePatternDesc.New({
        
        directions = {
            vec1,
            vec2
        }
    });
    mdt = MovementDescType.New({
        tpd = tp,
        range = {
            min = 1,
            max = -1;
        },
        tileCT = {
            {id = 1, cost = 1}
        },
        unitCT = {
            {id = 1, cost = -1}
        },
        maxGas = -1;
    });

    print("Tp Created");

end

function Undo(game)

end