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
            "Pick a GIF to import as animated pixel art.\n"
            "Each frame gets stacked on the same grid and animates when you play the level.",
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
            CCSprite::create(Mod::get()->getResourcesDir().append("icon.png").string().c_str()),
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
        log::warn("[VideoImporter] LuauAPI not ready yet.");
        return;
    }

    auto res = lua::runFile(Mod::get()->getResourcesDir(), "VideoImporter.luau");
    if (res.isErr()) {
        log::error("[VideoImporter] Luau error: {}", res.unwrapErr());
    }
}