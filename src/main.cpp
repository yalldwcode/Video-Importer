#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <imes.luauapi/include/LuauAPI.hpp>

#include "importer/VideoImporter.h"

using namespace geode::prelude;
namespace lua = imes::luauapi;

class $modify(VideoImporterUI, EditorUI) {
public:

    void onVideoImport(CCObject*) {
        if (this->getSelectedObjects()->count() != 1) {
            FLAlertLayer::create(
                "Video Importer",
                "Select <cy>exactly one</c> object first.\nIt'll be used as the anchor point for the frames.",
                "OK"
            )->show();
            return;
        }

        auto anchor = CCArrayExt<GameObject*>(this->getSelectedObjects())[0];
        auto imp = new VideoImporter();
        imp->setSelectedObject(anchor);
        imp->setCloseMenu([imp]() { delete imp; });

        geode::createQuickPopup(
            "Video Importer",
            "Pick a GIF to import as pixel art.\nEach frame gets placed side by side from your anchor.",
            "Cancel", "Choose File",
            [imp](auto, bool confirmed) {
                if (confirmed) {
                    imp->updateSettings();
                    imp->importVideo();
                } else {
                    delete imp;
                }
            }
        );
    }

    void createMoveMenu() {
        EditorUI::createMoveMenu();

        auto btn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png"),
            this,
            menu_selector(VideoImporterUI::onVideoImport)
        );
        btn->setID("video-import-btn"_spr);

        m_editButtonBar->m_buttonArray->addObject(btn);

        int rows = GameManager::sharedState()->getIntGameVariable("0049");
        int cols  = GameManager::sharedState()->getIntGameVariable("0050");
        m_editButtonBar->reloadItems(rows, cols);
    }
};

$on_mod(Loaded) {
    if (!lua::isReady()) {
        log::warn("[VideoImporter] LuauAPI isn't ready yet, skipping Luau load.");
        return;
    }

    auto res = lua::runFile(Mod::get()->getResourcesDir(), "VideoImporter.luau");
    if (res.isErr()) {
        log::error("[VideoImporter] Luau script error: {}", res.unwrapErr());
    }
}