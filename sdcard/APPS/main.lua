
function SetupUI()
    button = CreateButton()

    button:SetRect(30, 80, 80, 30)
    button:SetText("Start Radio")

    button:OnClick(function () button:SetText("WOAH!") end)
end

SetupUI();
