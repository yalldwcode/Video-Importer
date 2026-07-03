

#define STB_IMAGE_IMPLEMENTATION
#include "VideoImporter.h"

void VideoImporter::importVideo() {
    async::spawn(
        file::pick(file::PickMode::OpenFile, ALLOWED_TYPES),
        [this](Result<std::optional<std::filesystem::path>> result) {
            if (!result.isOk()) return;
            auto opt = result.unwrap();
            if (!opt) return;
            placeVideo(utils::string::pathToString(opt.value()));
        }
    );
}

void VideoImporter::placeVideo(const std::string& path) {

    FILE* f = fopen(path.c_str(), "rb");
    if (!f) {
        FLAlertLayer::create(
            "Video Importer",
            "Couldn't open the file.\nMake sure the path doesn't have any weird characters.",
            "OK"
        )->show();
        return;
    }

    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);
    rewind(f);

    std::vector<unsigned char> fileBuffer(fileSize);
    fread(fileBuffer.data(), 1, fileSize, f);
    fclose(f);


    int width = 0, height = 0, frames = 0, channels = 0;
    int* delays = nullptr;

    unsigned char* gifData = stbi_load_gif_from_memory(
        fileBuffer.data(),
        static_cast<int>(fileSize),
        &delays,
        &width, &height,
        &frames, &channels,
        0
    );

    if (!gifData) {
        FLAlertLayer::create(
            "Video Importer",
            "Failed to decode the GIF.\nMake sure it's a valid .gif file.",
            "OK"
        )->show();
        return;
    }

    
    int totalFrames = std::min(frames, frameLimit);

    int pixelsPerFrame = width * height;
    if (!noLimit && pixelsPerFrame > sizeLimit) {
        stbi_image_free(gifData);
        FLAlertLayer::create(
            "Video Importer",
            fmt::format(
                "This GIF has <cy>{}</c> pixels per frame but your limit is <cy>{}</c>.\n"
                "Try a smaller GIF or raise the limit in mod settings.",
                pixelsPerFrame, sizeLimit
            ),
            "OK"
        )->show();
        return;
    }

    

    std::ostringstream objStream;

    float anchorX = obj->getPositionX();
    float anchorY = obj->getPositionY();

    float frameWidth = width * pixelSize;

    for (int frameIndex = 0; frameIndex < totalFrames; ++frameIndex) {
       
        const unsigned char* frameData = gifData + (frameIndex * width * height * channels);

        float startX = anchorX + frameIndex * (frameWidth + frameSpacing);
        float startY = anchorY;

        placeFrame(objStream, frameData, width, height, channels, startX, startY);
    }

    stbi_image_free(gifData);

    std::string objString = objStream.str();
    if (objString.empty()) {
        FLAlertLayer::create("Video Importer", "No visible pixels found in this GIF.", "OK")->show();
        return;
    }
    objString.pop_back();

    LevelEditorLayer::get()->createObjectsFromString(objString.c_str(), true, true);

    FLAlertLayer::create(
        "Video Importer",
        fmt::format(
            "Done! Imported <cg>{}</c> frame{}.",
            totalFrames,
            totalFrames == 1 ? "" : "s"
        ),
        "OK"
    )->show();

    if (closeMenu) closeMenu();
}

void VideoImporter::placeFrame(
    std::ostringstream& out,
    const unsigned char* frameData,
    int width,
    int height,
    int channels,
    float startX,
    float startY
) {


    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            int idx = (y * width + x) * channels;

            
            uint8_t alpha = (channels == 4) ? frameData[idx + 3] : 255;
            if (alpha == 0) continue;

            std::string colour;
            formatHSV(frameData[idx], frameData[idx + 1], frameData[idx + 2], colour);

            
            out << "1," << pixelObjID
                << ",2," << startX + x * pixelSize
                << ",3," << startY - (height - 1 - y) * pixelSize
                << ",21," << colourChan
                << ",41,1,43," << colour
                << ",25," << zOrder
                << ",32," << pixelSize
                << ";";
        }
    }
}

void VideoImporter::formatHSV(float r, float g, float b, std::string& out) const {
    RGBtoHSV(r, g, b);
    out = std::to_string(r) + "a" + std::to_string(g) + "a" + std::to_string(b) + "a1a1";
}


void VideoImporter::RGBtoHSV(float& r, float& g, float& b) const {
    r /= 255.0f;
    g /= 255.0f;
    b /= 255.0f;

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

    r = hue;
    g = sat;
    b = val;
}

void VideoImporter::updateSettings() {
    noLimit      = Mod::get()->getSettingValue<bool>("disable-limit");
    sizeLimit    = static_cast<int>(Mod::get()->getSettingValue<int64_t>("size-limit"));
    colourChan   = static_cast<int>(Mod::get()->getSettingValue<int64_t>("colour-channel"));
    frameLimit   = static_cast<int>(Mod::get()->getSettingValue<int64_t>("frame-limit"));
    frameSpacing = static_cast<float>(Mod::get()->getSettingValue<double>("frame-spacing"));
    pixelSize    = static_cast<float>(Mod::get()->getSettingValue<double>("pixel-size"));
}