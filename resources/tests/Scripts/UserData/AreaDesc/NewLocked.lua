

local function getOne()
    return 1
end

function Execute(game)

    up = Vector2.new(0, 1);
    right = {x = 1, y = 0};
    left = Vector2.new(-1, 0);
    tp = AreaDesc.New({
        
        directions = {
            up,
            right
        },
        lockedDirs = {
            {
                dir = up, 
                locksTo = {
                    up,
                    right,
                    left
                }
            },
            {
                dir = right, 
                locksTo = {
                    up,
                    right
                }
            }
        }
    });
    print("Tp Created");

end

function Undo(game)

end