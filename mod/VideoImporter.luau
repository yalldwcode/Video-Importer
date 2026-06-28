-- VideoImporter.luau
-- This runs on mod load via LuauAPI.
-- Right now it handles editor-open logging and a small dev panel.
-- As the mod grows, move more UI and logic into Luau here.

local modId  = geode.Mod.getID()
local modVer = geode.Mod.getVersion()

-- Log whenever the level editor opens.
-- Useful for debugging during development.
geode.hook("geode.gd.LevelEditorLayer:init/2", {
    after = function(self, level, isNew, result)
        if result then
            print(string.format("[VideoImporter] Editor opened. Level: %s", tostring(level)))
        end
        return result
    end,
})

-- Small dev info panel — only useful while you're building the mod.
-- Feel free to expand this into a proper settings/status overlay later,
-- or remove it once things are stable.
local showPanel = false

imgui.onDraw(function()
    -- Toggle the panel with a checkbox in the ImGui demo window.
    -- In a real release you'd probably remove this or put it behind developer mode.
    imgui.window("VideoImporter Dev", function()
        imgui.text(modId .. " v" .. modVer)
        imgui.separator()
        imgui.text("Go into the editor, select one")
        imgui.text("object, and click the import button.")
        imgui.separator()
        imgui.text("Supported formats: .gif, .mp4, .webm, .mov")
        imgui.text("(Full video decode coming soon!)")
    end)
end)

print(string.format("[VideoImporter] Luau script loaded — %s %s", modId, modVer))
