

local function getOne()
    return 1
end

function Execute(game)

    vec1 = Vector2.new(0, 1);
    vec2 = Vector2.new(1, 0);
    tp = AreaDesc.New({
        
        directions = {
            vec1,
            vec2
        }
    });
    print("Tp Created");

end

function Undo(game)

end