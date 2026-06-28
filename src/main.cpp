#include <Geode/Geode.hpp>
#include <Geode/loader/ModEvent.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/async.hpp>
#include <imes.luauapi/include/LuauAPI.hpp>

using namespace geode::prelude;
namespace lua = imes::luauapi;

static const file::FilePickOptions VIDEO_TYPES = {
    std::nullopt,
    {
        { "Video / GIF", { "*.gif", "*.mp4", "*.webm", "*.mov" } }
    }
};


struct ImportSettings {
    int   frameLimit   = Mod::get()->getSettingValue<int64_t>("frame-limit");
    float frameSpacing = Mod::get()->getSettingValue<double>("frame-spacing");
    int   colourChan   = Mod::get()->getSettingValue<int64_t>("colour-channel");
    float pixelSize    = Mod::get()->getSettingValue<double>("pixel-size");
    int   sizeLimit    = Mod::get()->getSettingValue<int64_t>("size-limit");
    bool  noLimit      = Mod::get()->getSettingValue<bool>("disable-limit");
};


static void doImport(
    const std::filesystem::path& videoPath,
    GameObject* anchor,
    const ImportSettings& cfg
) {
    log::info("[VideoImporter] File selected: {}", videoPath.string());
    log::info("[VideoImporter] Frame limit: {}, Spacing: {}", cfg.frameLimit, cfg.frameSpacing);

    
    FLAlertLayer::create(
        "Video Importer",
        fmt::format(
            "File: <cg>{}</c>\n\n"
            "Frame limit: <cy>{}</c> | Spacing: <cy>{}</c>\n\n"
            "<cr>Frame decoding isn't implemented yet.</c>\n"
            "This scaffold is just proving the button and\nfile picker work. Add the decoder next!",
            videoPath.filename().string(),
            cfg.frameLimit,
            cfg.frameSpacing
        ),
        "OK"
    )->show();
}


class $modify(VideoImporterUI, EditorUI) {
public:

    void onVideoImport(CCObject*) {
        
        
        if (this->getSelectedObjects()->count() != 1) {
            FLAlertLayer::create(
                "Video Importer",
                "Select <cy>exactly one</c> object first.\n"
                "It will be used as the anchor point for the imported frames.",
                "OK"
            )->show();
            return;
        }

        auto anchor = CCArrayExt<GameObject*>(this->getSelectedObjects())[0];
        ImportSettings cfg;

        async::spawn(
            file::pick(file::PickMode::OpenFile, VIDEO_TYPES),
            [anchor, cfg](Result<std::optional<std::filesystem::path>> result) {
                if (!result.isOk()) return;
                auto opt = result.unwrap();
                if (!opt) return;
                doImport(opt.value(), anchor, cfg);
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
