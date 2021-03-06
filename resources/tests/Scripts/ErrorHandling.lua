


function Execute(game)

    local map = game:GetMap(mapIndex); -- mapIndex is defined int this env
    local unit = map:GetUnit(origin); -- origin is defined in this env

    print(origin);
    print(dest);

    if unit then
        if origin ~= dest then
            local move = unit:CalculateMovement(map, origin);
            if move:CanMove(dest) then -- dest is also defined in this env
                
                destUnit = map:GetUnit(dest);
                if destUnit then
                    map:RemoveUnit(dest);
                end
                
                print("Before removing unit")

                --map:RemoveUnit(origin);
                --map:AddUnit(origin, unit);
            else
                error ("Unit cannot move to that location")

            end
        else
            error("Origin and destination are the same position")
        end
    else
        error("Unit not found")

    end

end

function Undo(game)

    local map = game:GetMap(mapIndex);

    map:RemoveUnit(dest);
    map:AddUnit(origin, unit);
    
    if destUnit then
        map:AddUnit(dest, destUnit)
    end

end