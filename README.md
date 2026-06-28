# Video Importer

A Geometry Dash mod that imports video frames into the level editor as pixel art. Built with the [Geode SDK](https://geode-sdk.org) and powered by [LuauAPI](https://github.com/ItMe12s/LuauAPI).

## How to use

Open the level editor and go to the Edit tab. Select exactly one object — that's the anchor point where the frames start. Click the import button, pick a video or GIF file, and the frames will be placed in the editor side by side.

## Requirements

You need Geometry Dash 2.2081 and [Geode](https://geode-sdk.org) 5.7.1 or newer. LuauAPI gets installed automatically as a dependency.

## Building

To build locally with the Geode CLI just run `geode build`. The GitHub Actions workflow handles multi-platform builds — create `.github/workflows/build.yml` yourself using the Geode multi-platform template.

## Settings

You can adjust things like the frame limit, spacing between frames, pixel size, and colour channel from the mod settings. There's also a pixel limit per frame as a safety cap — raising it too high can crash the game on large videos.

## Credits

Inspired by [Art Importer](https://github.com/CelestialGecko/Art-importer) by Grian. Powered by [LuauAPI](https://github.com/ItMe12s/LuauAPI) by ItMe12s.
