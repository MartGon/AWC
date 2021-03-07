

function Execute(game)

    unit = map:GetUnit(origin);
    map:RemoveUnit(origin);
    if map:GetUnit(origin) == nil then
        success = true;
    end

end

function Undo(game)

end