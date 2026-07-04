#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/async.hpp>
#include <thread>
#include <sstream>

using namespace geode::prelude;

static const file::FilePickOptions ALLOWED_TYPES = {
    std::nullopt,
    {
        { "GIF / Video", { "*.gif", "*.mp4", "*.mov", "*.webm" } }
    }
};

class VideoImporter final {
private:
    GameObject* obj = nullptr;
    std::function<void()> closeMenu = nullptr;

    bool  noLimit    = false;
    int   sizeLimit  = 10000;
    int   colourChan = 1010;
    int   frameLimit = 30;
    float animFps    = 10.0f;
    float pixelSize  = 5.0f;
    int   baseGroup  = 100;
    bool  camFollow  = false;

    void placeVideo(const std::string& path);

public:
    VideoImporter() {}
    ~VideoImporter() {}

    void importVideo();
    void updateSettings();

    void setSelectedObject(GameObject* o) { obj = o; }
    void setCloseMenu(std::function<void()> c) { closeMenu = c; }
};