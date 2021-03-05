

function Execute(game)

    local unitMove = unit:CalculateMovement(map, origin);
    print(unitMove)
    if unitMove ~= nil then
        calculated = true;
    end

end

function Undo(game)

end