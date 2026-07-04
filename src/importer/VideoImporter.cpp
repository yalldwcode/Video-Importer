#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "VideoImporter.h"
#include <fstream>

void VideoImporter::importVideo() {
    async::spawn(
        file::pick(file::PickMode::OpenFile, ALLOWED_TYPES),
        [this](Result<std::optional<std::filesystem::path>> result) {
            if (!result.isOk()) return;
            auto opt = result.unwrap();
            if (!opt.has_value()) return;

            auto path = opt.value();
            auto ext  = path.extension().string();

            if (ext == ".mp4" || ext == ".mov" || ext == ".webm") {
                FLAlertLayer::create(
                    "Video Importer",
                    fmt::format(
                        "<cr>{}</c> files aren't supported yet.\n"
                        "Convert it to a GIF first at <cy>emotesizer.com/tools/resize</c>.",
                        ext
                    ),
                    "OK"
                )->show();
                return;
            }

            placeVideo(utils::string::pathToString(path));
        }
    );
}

void VideoImporter::placeVideo(const std::string& path) {
    auto winSize = CCDirector::get()->getWinSize();
    auto overlay = CCLayerColor::create({0, 0, 0, 160}, winSize.width, winSize.height);
    auto label   = CCLabelBMFont::create("Importing...", "bigFont.fnt");
    label->setPosition(winSize / 2);
    label->setScale(0.7f);
    overlay->addChild(label);
    CCScene::get()->addChild(overlay, 9999, 9999);

    float anchorX = obj->getPositionX();
    float anchorY = obj->getPositionY();

    int   fl = frameLimit;
    float ps = pixelSize;
    int   cc = colourChan;
    bool  nl = noLimit;
    int   sl = sizeLimit;
    float fps = animFps;
    int   bg = baseGroup;
    auto  cm = closeMenu;

    std::thread([path, anchorX, anchorY, fl, ps, cc, nl, sl, fps, bg, overlay, cm]() {
        std::ifstream file(path, std::ios::binary);
        if (!file) {
            Loader::get()->queueInMainThread([overlay]() {
                overlay->removeFromParent();
                FLAlertLayer::create("Video Importer", "Couldn't open the file.", "OK")->show();
            });
            return;
        }

        std::vector<unsigned char> buf(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()
        );
        file.close();

        int width = 0, height = 0, frames = 0, channels = 0;
        int* delays = nullptr;

        unsigned char* gifData = stbi_load_gif_from_memory(
            buf.data(), static_cast<int>(buf.size()),
            &delays, &width, &height, &frames, &channels, 0
        );

        if (!gifData) {
            Loader::get()->queueInMainThread([overlay]() {
                overlay->removeFromParent();
                FLAlertLayer::create("Video Importer", "Failed to decode the GIF.", "OK")->show();
            });
            return;
        }

        int totalFrames   = std::min(frames, fl);
        int pixPerFrame   = width * height;

        if (!nl && pixPerFrame > sl) {
            stbi_image_free(gifData);
            Loader::get()->queueInMainThread([overlay, pixPerFrame, sl]() {
                overlay->removeFromParent();
                FLAlertLayer::create(
                    "Video Importer",
                    fmt::format(
                        "This GIF has <cy>{}</c> pixels per frame but your limit is <cy>{}</c>.\n"
                        "Try a smaller GIF or raise the limit in mod settings.\n"
                        "You can resize at <cy>emotesizer.com/tools/resize</c>.",
                        pixPerFrame, sl
                    ),
                    "OK"
                )->show();
            });
            return;
        }

        std::ostringstream objStream;
        float cx = anchorX;
        float cy = anchorY;

        for (int i = 0; i < totalFrames; ++i) {
            const unsigned char* frameData = gifData + (i * width * height * channels);
            int groupId = bg + i;

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    int idx   = (y * width + x) * channels;
                    uint8_t a = (channels == 4) ? frameData[idx + 3] : 255;
                    if (a == 0) continue;

                    std::string colour;
                    float r = frameData[idx], g = frameData[idx + 1], b = frameData[idx + 2];

                    r /= 255.0f; g /= 255.0f; b /= 255.0f;
                    float maxC = std::max(r, std::max(g, b));
                    float minC = std::min(r, std::min(g, b));
                    float delta = maxC - minC;
                    float hue = 0.0f, sat = 0.0f, val = maxC;
                    if (delta > 0.0f) {
                        sat = delta / maxC;
                        if      (maxC == r) hue = 60.0f * ((g - b) / delta);
                        else if (maxC == g) hue = 60.0f * (((b - r) / delta) + 2.0f);
                        else                hue = 60.0f * (((r - g) / delta) + 4.0f);
                    }
                    if (hue < 0.0f) hue += 360.0f;
                    hue = ((static_cast<int>(hue) + 180) % 360) - 180;
                    if (hue == 0.0f) hue++;
                    colour = std::to_string(hue) + "a" + std::to_string(sat) + "a" + std::to_string(val) + "a1a1";

                    float gdX = cx + (x - width  / 2.0f) * ps;
                    float gdY = cy + (height / 2.0f - y) * ps;

                    objStream << "1," << 3097
                        << ",2," << gdX
                        << ",3," << gdY
                        << ",21," << cc
                        << ",41,1,43," << colour
                        << ",25," << 1
                        << ",32," << ps
                        << ",57," << groupId
                        << ";";
                }
            }
        }

        stbi_image_free(gifData);

        float frameDuration = 1.0f / fps;
        int setupGroup = bg + totalFrames;

        float tx = anchorX - 300.0f;
        float ty = anchorY;

        objStream << "1,1268,2," << (tx - 50.0f) << ",3," << ty << ",51," << setupGroup << ",63,0;";

        for (int j = 1; j < totalFrames; ++j) {
            objStream << "1,1049,2," << tx << ",3," << ty
                << ",57," << setupGroup << ",58,1"
                << ",51," << (bg + j) << ",56,0;";
        }

        for (int i = 0; i < totalFrames; ++i) {
            int cycleGroup     = setupGroup + 1 + i;
            int nextCycleGroup = setupGroup + 1 + ((i + 1) % totalFrames);
            int hideGroup      = bg + i;
            int showGroup      = bg + ((i + 1) % totalFrames);

            objStream << "1,1268,2," << tx << ",3," << ty
                << ",57," << setupGroup << ",58,1"
                << ",51," << cycleGroup << ",63,0;";

            objStream << "1,1049,2," << tx << ",3," << ty
                << ",57," << cycleGroup << ",58,1"
                << ",51," << hideGroup << ",56,0;";

            objStream << "1,1049,2," << tx << ",3," << ty
                << ",57," << cycleGroup << ",58,1"
                << ",51," << showGroup << ",56,1;";

            objStream << "1,1268,2," << tx << ",3," << ty
                << ",57," << cycleGroup << ",58,1"
                << ",51," << nextCycleGroup << ",63," << frameDuration << ";";
        }

        std::string objString = objStream.str();
        if (!objString.empty() && objString.back() == ';')
            objString.pop_back();

        Loader::get()->queueInMainThread([objString, totalFrames, overlay, cm]() {
            overlay->removeFromParent();

            auto* lel = LevelEditorLayer::get();
            if (!lel) {
                FLAlertLayer::create("Video Importer", "The editor closed during import.", "OK")->show();
                return;
            }

            lel->createObjectsFromString(objString.c_str(), true, true);

            FLAlertLayer::create(
                "Video Importer",
                fmt::format("Done! Imported <cg>{}</c> frame{}.", totalFrames, totalFrames == 1 ? "" : "s"),
                "OK"
            )->show();

            if (cm) cm();
        });
    }).detach();
}

void VideoImporter::updateSettings() {
    noLimit    = Mod::get()->getSettingValue<bool>("disable-limit");
    sizeLimit  = static_cast<int>(Mod::get()->getSettingValue<int64_t>("size-limit"));
    colourChan = static_cast<int>(Mod::get()->getSettingValue<int64_t>("colour-channel"));
    frameLimit = static_cast<int>(Mod::get()->getSettingValue<int64_t>("frame-limit"));
    animFps    = static_cast<float>(Mod::get()->getSettingValue<double>("anim-fps"));
    pixelSize  = static_cast<float>(Mod::get()->getSettingValue<double>("pixel-size"));
    baseGroup  = static_cast<int>(Mod::get()->getSettingValue<int64_t>("base-group"));
}