#pragma once
#include <Geode/Geode.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/async.hpp>

#include "stb_image.h"

using namespace geode::prelude;

static auto ALLOWED_TYPES = file::FilePickOptions{
    std::nullopt,
    {
        {
            "GIF Files",
            { "*.gif" }
        }
    }
};

class VideoImporter final {
private:
    
    static constexpr int pixelObjID = 3097;
    static constexpr int zOrder     = 1;

    
    GameObject* obj = nullptr;

    
    std::function<void()> closeMenu = nullptr;

    
    bool noLimit     = false;
    int  sizeLimit   = 10000;
    int  colourChan  = 1010;
    int  frameLimit  = 30;
    float frameSpacing = 50.0f;
    float pixelSize  = 5.0f;

    
    void placeVideo(const std::string& path);
    void placeFrame(
        std::ostringstream& out,
        const unsigned char* frameData,
        int width,
        int height,
        int channels,
        float startX,
        float startY
    );
    void formatHSV(float r, float g, float b, std::string& out) const;
    void RGBtoHSV(float& r, float& g, float& b) const;

public:
    VideoImporter() {}
    ~VideoImporter() {}

    void importVideo();
    void updateSettings();

    void setSelectedObject(GameObject* o) { obj = o; }
    void setCloseMenu(std::function<void()> c) { closeMenu = c; }
};