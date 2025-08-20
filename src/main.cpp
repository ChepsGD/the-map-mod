/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/casts.hpp>

#include <Geode/binding/FMODAudioEngine.hpp>
#include "cocos-ext.h"
USING_NS_CC_EXT;

using namespace geode::prelude;

class TheMapLayer : public cocos2d::CCLayer {
public:
	CREATE_FUNC(TheMapLayer);
    virtual bool init() override {
        if (!CCLayer::init()) return false;

		// Handle the BGM
		FMODAudioEngine::sharedEngine()->stopAllMusic(0);
    	FMODAudioEngine::sharedEngine()->playMusic("secretLoop.mp3", true, 1, 0);

		// Handle loading assets
		auto bg = cocos2d::CCSprite::create("mapbg.png");
		bg->setPosition({ 240, 160 }); // center of 480x320 scene
		auto spr = CircleButtonSprite::createWithSpriteFrameName("GJ_moveBtn-hd.png"_spr);

    	auto btn = CCMenuItemSpriteExtra::create(
        	spr, this, menu_selector(TheMapLayer::onButton)
    	);
		btn->setPosition({240, 160});

		auto menu = CCMenu::create();
		menu->addChild(btn);
		menu->setPosition({0, 0});
		
		// Scrolling support
		auto windowSize = CCDirector::sharedDirector()->getWinSize();
		auto scrolLayer = CCLayer::create();
		scrolLayer->addChild(bg, -1);
		scrolLayer->addChild(menu, 0);
		auto scroll = CCScrollView::create({windowSize.width, windowSize.height}, scrolLayer);
		scroll->setDirection(kCCScrollViewDirectionBoth);  
		scroll->setTouchEnabled(true);
		scroll->setContentSize({ windowSize.width * 2, windowSize.height * 2 });
		this->addChild(scroll);

		// Handle loading assets that dont scroll and show above it.
		auto label = cocos2d::CCLabelBMFont::create("The Map", "goldFont.fnt");
        label->setPosition({ 275, 300 }); // center of 480x320 scene
        this->addChild(label);
		
        // Initialize SomeNode
        return true;
    }
	void onButton(CCObject*) {
		auto lvl1ID = Mod::get()->getSettingValue<int64_t>("level-one-id");
		auto level = GameLevelManager::sharedState()->getMainLevel(lvl1ID, false);

		auto lvlLayer = LevelInfoLayer::create(level, false); // example id idk
		lvlLayer->downloadLevel();
		auto lvlScene = CCScene::create();
		lvlScene->addChild(lvlLayer);
		CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, lvlScene));
	}

	virtual void keyBackClicked() {
		auto goBack = CCScene::create();
		goBack->addChild(CreatorLayer::create);
		CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, goBack));
	}
};

#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/utils/web.hpp>
class $modify(MyCreatorLayer, CreatorLayer) {
    bool init() override {
        if (!CreatorLayer::init()) return false;

        auto menu = static_cast<CCMenu*>(this->getChildByID("creator-buttons-menu"));
        if (!menu) return true;

        return true;
    }
	void onAdventureMap(CCObject*) {
		auto myLayer = TheMapLayer::create();
		
		auto scene = cocos2d::CCScene::create();
    	scene->addChild(myLayer);
		
		auto transition = cocos2d::CCTransitionFade::create(0.5f, scene);
		cocos2d::CCDirector::sharedDirector()->replaceScene(transition);
	} 
};
