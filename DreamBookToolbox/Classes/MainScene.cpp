#include "MainScene.h"
#include "PhysicsTechScene.h"
#include "DreamBookScene.h"
#include "ToyBrickScene.h"

USING_NS_CC;

void MainLayer::menuDreamBookCallback( CCObject* pSender )
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0f, DreamBookLayer::scene(), false));
}

void MainLayer::menuPhysicsTechCallback( CCObject* pSender )
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0f, PhysicsTechScene::create(), false));
}

void MainLayer::menuToyBrickCallback( CCObject* pSender )
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0f, ToyBrickScene::create(), false));
}

void MainLayer::menuCloseCallback( CCObject* pSender )
{
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->removeSpriteFramesFromFile("PlayBox.plist");

    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

bool MainLayer::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        cache->addSpriteFramesWithFile("PlayBox.plist", "PlayBox.png");

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemSprite *pCloseItem = CCMenuItemSprite::create(
            CCSprite::createWithSpriteFrameName("close.png"), NULL, this,
            menu_selector(MainLayer::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        CCSize winSize = CCDirector::sharedDirector()->getWinSize();

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(winSize.width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to Main layer as a child layer.
        this->addChild(pMenu, 1);

        CCLabelTTF* label = CCLabelTTF::create("DreamBook", "Arial", 24);
        CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(MainLayer::menuDreamBookCallback));
        pMenu = CCMenu::create(pMenuItem, NULL);

        label = CCLabelTTF::create("PhysicsTech", "Arial", 24);
        pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(MainLayer::menuPhysicsTechCallback));
        pMenu->addChild(pMenuItem);

        label = CCLabelTTF::create("ToyBrick", "Arial", 24);
        pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(MainLayer::menuToyBrickCallback));
        pMenu->addChild(pMenuItem);

        pMenu->alignItemsVertically();
        this->addChild(pMenu);

        pMenu->setPosition(ccp(winSize.width * 0.5f - 20.0f, winSize.height * 0.8f));
        CC_BREAK_IF(! pMenu);

        bRet = true;
    } while (0);

    return bRet;
}

cocos2d::CCScene* MainLayer::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        MainLayer *layer = MainLayer::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}