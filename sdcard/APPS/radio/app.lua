
function SetupUI()
    local app = {}

    app.button = CreateButton();
    app.button:SetRect(1*8, 3*16, 24*8, 2*16)
    app.button:SetText("RunSDOverUSB")
    app.button:OnClick(function () GetFirmware():RunSDOverUSB() end)
end

-- function SetupHackRfOne()
--     hackRf = GetHackRfOne();

--     local sampleRate = 6000000;
--     local fmRate = 240000;
--     local audioRate = 48000;
    
--     hackRf:SetFrequency(103000000);
--     hackRf:SetSampleRate(sampleRate);
--     hackRf:SetBandwidth(10750000);
--     hackRf:SetLNA(40);
--     hackRf:SetVGA(24);
--     hackRf:SetAMP(1);
    
--     local hackRfOutputStream = hackRf:GetOutputStream();

--     local decimatedStream = CreateDecimatingLowPassFirFilter { 
--         inputStream = hackRfOutputStream,
--         decimation = sampleRate / fmRate,
--         sampleRate = sampleRate,
--         cutoffFrequency = 290000,
--         filterPower = 16
--     };

--     local fmStream = CreateFMDecoder {
--         inputStream = decimatedStream,
--         sampleRate = fmRate,
--         audioDecimation = fmRate / audioRate,
--     };

--     local audioStream = CreateDcBlocker {
--         inputStream = fmStream,
--     }

--     CreateAudioSink(audioStream);
-- end

-- function OnFrame()

-- end

mainApp = SetupUI();
--SetupHackRfOne();
--RegisterEvent("ON_FRAME", OnFrame);

