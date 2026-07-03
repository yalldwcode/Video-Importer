#pragma once
#include <Geode/Bindings.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/Utils.hpp>
#include "VideoImporter.h"

using namespace geode::prelude;

class ImportMenu : public geode::Popup<> {
private:
    VideoImporter* importer = nullptr;

protected:
    bool init(VideoImporter* imp);

    void onImport(CCObject*);
    void onInfo(CCObject*);
    void onSettings(CCObject*) { openSettingsPopup(Mod::get()); }

public:
    static ImportMenu* create(VideoImporter* imp);
};