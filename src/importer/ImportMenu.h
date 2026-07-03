#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "VideoImporter.h"

using namespace geode::prelude;

class ImportMenu : public geode::Popup<VideoImporter*> {
protected:
    VideoImporter* m_importer = nullptr;

    bool setup(VideoImporter* imp) override;

    void onImport(CCObject*);
    void onInfo(CCObject*);
    void onSettings(CCObject*) { openSettingsPopup(Mod::get()); }

public:
    static ImportMenu* create(VideoImporter* imp);
};