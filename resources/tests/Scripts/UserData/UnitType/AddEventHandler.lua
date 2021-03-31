local soldierType = DB:GetUnitType(0);

soldierType:AddEventHandler({
    opType = 1,
    callback = function ()
        Check()
        print("Hello from an event handler")
    end,
    notiType = 4
})

unit = soldierType:CreateUnit(owner);