function SetupUI()
    local app = {}

    app.button1 = CreateButton()
    app.button1:SetRect(1*8, 3*16, 24*8, 2*16)
    app.button1:SetText("Start SD over USB")
    app.button1:OnClick(function () GetFirmware():RunSDOverUSB() end)

    app.button2 = CreateButton()
    app.button2:SetRect(1*8, 5*16, 24*8, 2*16)
    app.button2:SetText("Start Flash Utility")
    app.button2:OnClick(function () GetFirmware():RunFlashUtility() end)

    app.button3 = CreateButton()
    app.button3:SetRect(1*8, 7*16, 24*8, 2*16)
    app.button3:SetText("Start HackRf mode")
    app.button3:OnClick(function () GetFirmware():RunHackRfMode() end)

    app.button5 = CreateButton()
    app.button5:SetRect(1*8, 9*16, 24*8, 2*16)
    app.button5:SetText("Take Screenshot")
    app.button5:OnClick(function () GetFirmware():TakeScreenshot() end)

    app.button6 = CreateButton()
    app.button6:SetRect(1*8, 11*16, 24*8, 2*16)
    app.button6:SetText("Start Radio")
    app.button6:OnClick(function ()
        GetFirmware():RunApp("/APPS/radio/app.lua")
        mainApp = nil -- free resources
    end)

    local firmware = GetFirmware()
    local free_heap = string.format("Free heap: %d", firmware:GetFreeHeap())
    app.label1 = CreateLabel()
    app.label1:SetTextAndPosition(free_heap, 2, 2)
    local free_stack = string.format("Free stack: %d", firmware:GetFreeStack())
    app.label2 = CreateLabel()
    app.label2:SetTextAndPosition(free_stack, 2, 20)

    return app
end

mainApp = SetupUI(); -- set reference so it does not get gc'ed
