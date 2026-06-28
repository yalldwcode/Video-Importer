

local modId  = geode.Mod.getID()
local modVer = geode.Mod.getVersion()


geode.hook("geode.gd.LevelEditorLayer:init/2", {
    after = function(self, level, isNew, result)
        if result then
            print(string.format("[VideoImporter] Editor opened. Level: %s", tostring(level)))
        end
        return result
    end,
})


local showPanel = false

imgui.onDraw(function()
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

print(string.format("[VideoImporter] Luau script loaded, %s %s", modId, modVer))
