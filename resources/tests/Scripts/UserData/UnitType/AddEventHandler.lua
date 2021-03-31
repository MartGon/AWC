local soldierType = DB:GetUnitType(0);
handled = false;
soldierType:AddEventHandler({
    opType = 1,
    callback = function ()
        handled = true;
        print("Hello from an event handler")
    end,
    notiType = 4
})

unit = soldierType:CreateUnit(owner);