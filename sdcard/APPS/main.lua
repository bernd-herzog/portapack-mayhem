function SetupUI()
    local button = CreateButton()

    button:SetRect(30, 80, 80, 30)
    button:SetText("A button")

    button:OnClick(function () button:SetText("WOAH!") end)

    local label = CreateLabel()
    label:SetTextAndPosition("Hello Lua", 2, 2)
end

SetupUI();
