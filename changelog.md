## Changelog

## v1.0.0-beta.2

Rewrote the mod to properly mirror Art Importer's structure. Added VideoImporter and ImportMenu classes, wired up the in-editor popup with an import button, settings, and info. GIF decoding is now implemented using stb_image. each frame gets placed as pixel objects side by side in the editor.

Still missing: custom button icon (using a placeholder for now).

## v1.0.0-beta.1

First beta! The import button shows up in the Edit tab, the file picker works for .gif, .mp4, .webm and .mov files, and all the mod settings are wired up. LuauAPI is set up with a companion Luau script for logging.

Actual frame decoding and pixel placement aren't done yet that's coming soon.