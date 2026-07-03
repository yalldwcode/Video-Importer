#include "ImportMenu.h"

bool ImportMenu::setup(VideoImporter* imp) {
    m_importer = imp;

    this->setTitle("Video Importer");

    auto importBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Choose a GIF"),
        this,
        menu_selector(ImportMenu::onImport)
    );
    importBtn->setPosition(m_size.width / 2, m_size.height / 2 + 7.5f);
    m_buttonMenu->addChild(importBtn);

    auto settingsBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Settings"),
        this,
        menu_selector(ImportMenu::onSettings)
    );
    settingsBtn->setPosition(m_size.width / 2, m_size.height / 2 - 32.5f);
    m_buttonMenu->addChild(settingsBtn);

    auto infoBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
        this,
        menu_selector(ImportMenu::onInfo)
    );
    infoBtn->setPosition(m_size.width, m_size.height);
    m_buttonMenu->addChild(infoBtn);

    return true;
}

ImportMenu* ImportMenu::create(VideoImporter* imp) {
    auto ret = new ImportMenu();
    if (ret->initAnchored(240.f, 130.f, imp)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void ImportMenu::onImport(CCObject*) {
    m_importer->updateSettings();
    m_importer->importVideo();
}

void ImportMenu::onInfo(CCObject*) {
    FLAlertLayer::create(
        "Video Importer",
        "Imports <cg>GIF</c> frames into the editor as pixel art.\n\n"
        "Select one object as your anchor point, then pick a GIF file.\n"
        "Each frame gets placed side by side from that anchor.",
        "OK"
    )->show();
}