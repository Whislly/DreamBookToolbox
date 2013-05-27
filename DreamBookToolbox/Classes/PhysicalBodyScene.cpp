#include "PhysicalBodyScene.h"
#include "MainScene.h"

USING_NS_CC;
using namespace cocos2d::extension;

void PhysicalBodyLayer::menuReturnCallback( CCObject* pSender )
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0f, MainLayer::scene(), true));
}

bool PhysicalBodyLayer::init()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    skeletonNode = CCSkeleton::createWithFile("spineboy.json", "spineboy.atlas");
    AnimationStateData_setMixByName(skeletonNode->state->data, "walk", "jump", 0.4f);
    AnimationStateData_setMixByName(skeletonNode->state->data, "jump", "walk", 0.4f);
    AnimationState_setAnimationByName(skeletonNode->state, "walk", true);
    skeletonNode->timeScale = 0.3f;
    skeletonNode->debugBones = false;

    /*skeletonNode->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(1),
    CCFadeIn::create(1),
    CCDelayTime::create(5),
    NULL)));*/

    CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
    skeletonNode->setPosition(ccp(windowSize.width / 2, 20));
    addChild(skeletonNode);

    scheduleUpdate();

    CCSprite* pReturn = CCSprite::createWithSpriteFrameName("Back.PNG");
    CCMenuItemSprite* pReturnMenuItem = CCMenuItemSprite::create(pReturn, NULL, this, menu_selector(PhysicalBodyLayer::menuReturnCallback));
    CCMenu* pMenu = CCMenu::createWithItem(pReturnMenuItem);
    addChild(pMenu);
    pMenu->setPosition(ccp(winSize.width - 50.0f, 50.f));


    return true;
}

cocos2d::CCScene* PhysicalBodyLayer::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        PhysicalBodyLayer *layer = PhysicalBodyLayer::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

PhysicalBodyLayer::PhysicalBodyLayer()
{

}

PhysicalBodyLayer::~PhysicalBodyLayer()
{

}

void PhysicalBodyLayer::update( float delta )
{
    /*if (skeletonNode->state->loop) {
    if (skeletonNode->state->time > 2) AnimationState_setAnimationByName(skeletonNode->state, "jump", false);
    } else {
    if (skeletonNode->state->time > 1) AnimationState_setAnimationByName(skeletonNode->state, "walk", true);
    }*/
    if (skeletonNode->state->time > 1) 
    AnimationState_setAnimationByName(skeletonNode->state, "walk", true);
}

void PhysicalBodyLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void PhysicalBodyLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool PhysicalBodyLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    return true;
}

void PhysicalBodyLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
    AnimationState_setAnimationByName(skeletonNode->state, "jump", false);
}
