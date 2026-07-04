local modId  = geode.Mod.getID()
local modVer = geode.Mod.getVersion()

geode.hook("geode.gd.LevelEditorLayer:init/2", {
    after = function(self, level, isNew, result)
        if result then
            print("Video Importer is ready in the editor")
        end
        return result
    end,
})

geode.hook("geode.gd.EditorUI:onPlaytest/2", {
    after = function(self, sender, result)
        print("Playtesting started, animation triggers should be running")
        return result
    end,
})

imgui.onDraw(function()
    imgui.window("Video Importer", function()
        imgui.text(modId .. " v" .. modVer)
        imgui.separator()
        imgui.text("Select one object in the editor,")
        imgui.text("then click the import button in the Edit tab.")
        imgui.separator()
        imgui.text("Keep GIFs small for best results.")
        imgui.text("emotesizer.com/tools/resize")
    end)
end)

print("Video Importer loaded " .. modVer)