function SetupUI()
    local button1 = CreateButton()
    button1:SetRect(1*8, 3*16, 24*8, 2*16)
    button1:SetText("Start SD over USB")
    button1:OnClick(function () CreateFirmware():RunSDOverUSB() end)

    local button2 = CreateButton()
    button2:SetRect(1*8, 5*16, 24*8, 2*16)
    button2:SetText("Start Flash Utility")
    button2:OnClick(function () CreateFirmware():RunFlashUtility() end)

    local button3 = CreateButton()
    button3:SetRect(1*8, 7*16, 24*8, 2*16)
    button3:SetText("Start HackRf mode")
    button3:OnClick(function () CreateFirmware():RunHackRfMode() end)

    local label = CreateLabel()
    label:SetTextAndPosition("Hello Lua", 2, 2)
end

SetupUI();
