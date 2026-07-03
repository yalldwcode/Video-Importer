#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "VideoImporter.h"

using namespace geode::prelude;

class ImportMenu : public geode::Popup<> {
protected:
    VideoImporter* m_importer = nullptr;

    bool init(VideoImporter* imp);

    void onImport(CCObject*);
    void onInfo(CCObject*);
    void onSettings(CCObject*) { openSettingsPopup(Mod::get()); }

public:
    static ImportMenu* create(VideoImporter* imp);
};