--require("string")

function SetupUI()
    local button1 = CreateButton()
    button1:SetRect(1*8, 3*16, 24*8, 2*16)
    button1:SetText("Start SD over USB")
    button1:OnClick(function () GetFirmware():RunSDOverUSB() end)

    local button2 = CreateButton()
    button2:SetRect(1*8, 5*16, 24*8, 2*16)
    button2:SetText("Start Flash Utility")
    button2:OnClick(function () GetFirmware():RunFlashUtility() end)

    local button3 = CreateButton()
    button3:SetRect(1*8, 7*16, 24*8, 2*16)
    button3:SetText("Start HackRf mode")
    button3:OnClick(function () GetFirmware():RunHackRfMode() end)

    local button4 = CreateButton()
    button4:SetRect(1*8, 9*16, 24*8, 2*16)
    button4:SetText("error")
    button4:OnClick(function () local u = "a"; u(); end)

    local button5 = CreateButton()
    button5:SetRect(1*8, 9*16, 24*8, 2*16)
    button5:SetText("Radio")
    button5:OnClick(function () GetFirmware():RunApp("radio") end)

    --local firmware = GetFirmware()

    --local freeHeap = 6 -- + firmware:GetFreeHeap()
    --local h = tostring(freeHeap)
    --local numstr = string.format("Free Heap: %f", 332.3)
    --CreateLabel():SetTextAndPosition("Free Heap: " .. 332.3 .. "bytes", 2, 2)
    CreateLabel():SetTextAndPosition(type(_ENV.string), 2, 2)
    --CreateLabel():SetTextAndPosition("Free Stack:" .. tostring(firmware:GetFreeStack()), 128, 2)
end

SetupUI();
