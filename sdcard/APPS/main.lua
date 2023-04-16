function SetupUI()
    local firmware = GetFirmware()

    local button1 = CreateButton()
    button1:SetRect(1*8, 3*16, 24*8, 2*16)
    button1:SetText("Start SD over USB")
    button1:OnClick(function () firmware:RunSDOverUSB() end)

    local button2 = CreateButton()
    button2:SetRect(1*8, 5*16, 24*8, 2*16)
    button2:SetText("Start Flash Utility")
    button2:OnClick(function () firmware:RunFlashUtility() end)

    local button3 = CreateButton()
    button3:SetRect(1*8, 7*16, 24*8, 2*16)
    button3:SetText("Start HackRf mode")
    button3:OnClick(function () firmware:RunHackRfMode() end)

    local button5 = CreateButton()
    button5:SetRect(1*8, 9*16, 24*8, 2*16)
    button5:SetText("Take Screenshot")
    button5:OnClick(function () firmware:TakeScreenshot() end)

    local free_heap = string.format("Free heap: %d", firmware:GetFreeHeap())
    CreateLabel():SetTextAndPosition(free_heap, 2, 2)
    local free_stack = string.format("Free stack: %d", firmware:GetFreeStack())
    CreateLabel():SetTextAndPosition(free_stack, 2, 20)
end

SetupUI();
