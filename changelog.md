## v1.0.0-beta.3

The animation was broken in beta.2. What was happening is the setup group was spawning all frame cycle groups at the same time with no delay, so every frame transition fired simultaneously. That's why it looked instant no matter what FPS you set in settings. It's fixed now each cycle group properly chains to the next one and the delay actually works.

There's now an optional camera follow setting. When you turn it on, a Follow trigger gets placed with the animation triggers so the camera locks onto the pixel art when the level plays. Good if your pixel art is far from where the player starts.

The Luau script now actually does something, it logs when the editor opens, hooks into playtesting, and shows a small info panel if you have the ImGui overlay open.

## v1.0.0-beta.2

GIF frames are now actually decoded and placed in the editor as pixel art. All frames go on the same grid and animate automatically when you play the level. The pixel art was coming out upside down in beta.1, that's fixed. Importing a big GIF now runs in the background with a loading screen. macOS and iOS builds added.

## v1.0.0-beta.1

First beta. The import button showed up and the file picker opened but nothing was actually imported yet.