

function Execute(game)

    map:AddUnit(origin, unit);
    
    if map:GetUnit(origin) then
        success = true;
    end

end

function Undo(game)

end