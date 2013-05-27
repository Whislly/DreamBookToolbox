#include "StoryScene.h"
#include "CCSpriteEx.h"
#include "MainScene.h"
#include "StoryChatLayer.h"

USING_NS_CC;

#define CaptionTag  5000
#define PearTag     5001
#define BkgTag      5002

void StoryLayer::loadChat( cocos2d::CCNode* pSender )
{
    removeChildByTag(PearTag, true);
    removeChildByTag(CaptionTag, true);
}

void StoryLayer::startStory( cocos2d::CCObject* pSender )
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCDirector::sharedDirector()->setDepthTest(true);
    CCNode* pBkgLayer = getChildByTag(BkgTag);
    getChildByTag(PearTag)->setVisible(false);
    getChildByTag(CaptionTag)->setVisible(false);
    pBkgLayer->runAction(CCSequence::create(CCPageTurn3D::create(1.0f, CCSizeMake(15,10)), CCCallFuncN::create(this, callfuncN_selector(StoryLayer::loadChat)), NULL));

    StoryChatLayer* pChatLayer = StoryChatLayer::create();
    addChild(pChatLayer);
    pChatLayer->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
    pChatLayer->runAction(CCFadeOut::create(1.0f));
}

void StoryLayer::addCaption(CCNode* pSender)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize borderSize = pSender->getContentSize();

    CCSprite* pCaptionBkg = CCSprite::createWithSpriteFrameName("middle.png");
    addChild(pCaptionBkg, 1, BkgTag);
    CCSize captionSize = pCaptionBkg->getContentSize();
    pCaptionBkg->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));

    //CCLabelTTF* pCaption = CCLabelTTF::create(CodeConvert::a2u(buff).c_str(), "Marker Felt", 40);
    CCLabelTTF* pCaption = CCLabelTTF::create("Kong Rong Gave Away", "Marker Felt", 40);
    addChild(pCaption, 1, CaptionTag);
    pCaption->setColor(ccBLACK);
    //EN
    //pCaption->setPosition(ccp(winSize.width * 0.5f - 40.0f, winSize.height * 0.5f));
    
    //CN
    pCaption->setPosition(ccp(winSize.width * 0.5f - 40, winSize.height * 0.5f));

    CCSpriteEx* pPear = CCSpriteEx::createWithSpriteFrameName("pear.png");
    addChild(pPear, 1, PearTag);
    pPear->setScale(0.5f);
    pPear->setSelectorForSingleClick(this, menu_selector(StoryLayer::startStory));
    //EN
    //pPear->setPosition(ccp(pCaption->getPositionX() + 165.0f, winSize.height * 0.5f));

    //CH
    pPear->setPosition(ccp(pCaption->getPositionX() + 220.0f, winSize.height * 0.5f));
    //pPear->runAction(CCRepeatForever::create(CCSequence::create(CCRotateBy::create(0.3f, 15), CCDelayTime::create(0.3f), CCRotateBy::create(0.6f, -30),  CCDelayTime::create(0.3f), CCRotateBy::create(0.3f, 15), NULL)));
    pPear->runAction(CCRepeatForever::create(CCSequence::create(CCRotateBy::create(0.3f, 15), CCRotateBy::create(0.6f, -30), CCRotateBy::create(0.3f, 15), NULL)));
}

void StoryLayer::menuReturnCallback( CCObject* pSender )
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionZoomFlipY::create(1.0f, MainLayer::scene()));
}

bool StoryLayer::init()
{
    CCSprite* pleft = CCSprite::createWithSpriteFrameName("left.png");
    CCSprite* pright = CCSprite::createWithSpriteFrameName("right.png");

    addChild(pleft);
    addChild(pright);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize borderSize = pleft->getContentSize();

    pleft->setPosition(ccp(-100, winSize.height * 0.3f));
    pright->setPosition(ccp(-53, winSize.height * 0.3f));

    CCSpawn* pMoveAction1 = CCSpawn::create(CCMoveBy::create(0.4f, ccp(winSize.width * 0.4f, winSize.height * 0.3f)), CCRotateBy::create(0.4f, 180), NULL);
    CCSpawn* pMoveAction2 = CCSpawn::create(CCMoveBy::create(0.4f, ccp(winSize.width * 0.5f, 0 - winSize.height * 0.5f - 47.0f)), CCRotateBy::create(0.4f, 240), NULL);
    CCSpawn* pMoveAction3 = CCSpawn::create(CCMoveTo::create(0.4f, ccp(winSize.width * 0.5f - borderSize.width * 0.5f, winSize.height * 0.5f)), CCRotateTo::create(0.4f, 0), NULL);
    CCActionInterval* pAction1 = CCSequence::create(pMoveAction1, CCDelayTime::create(0.4f), pMoveAction2, 
                                                    CCDelayTime::create(0.4f), pMoveAction3, CCDelayTime::create(0.4f), 
                                                    CCMoveBy::create(0.4f, ccp(0 - 257, 0)), NULL);
    
    CCSpawn* pMoveAction4 = CCSpawn::create(CCMoveTo::create(0.4f, ccp(winSize.width * 0.5f - borderSize.width * 0.5f + 47.0f, winSize.height * 0.5f)), CCRotateTo::create(0.4f, 0), NULL);
    CCActionInterval* pAction2 = CCSequence::create((CCActionInterval*)pMoveAction1->copy(), CCDelayTime::create(0.4f), 
                                                    (CCActionInterval*)pMoveAction2->copy(), CCDelayTime::create(0.4f), 
                                                    pMoveAction4, CCDelayTime::create(0.4f), CCMoveBy::create(0.4f, ccp(257, 0)), 
                                                    CCCallFuncN::create(this, callfuncN_selector(StoryLayer::addCaption)), NULL);
    pleft->runAction(pAction1);
    pright->runAction(pAction2);

    CCSprite* pReturn = CCSprite::createWithSpriteFrameName("pear.png");
    pReturn->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(1.0f), CCRotateBy::create(0.3f, 15), CCDelayTime::create(1.0f), CCRotateBy::create(0.3f, -15), NULL)));
    CCMenuItemSprite* pReturnMenuItem = CCMenuItemSprite::create(pReturn, NULL, this, menu_selector(StoryLayer::menuReturnCallback));
    CCMenu* pMenu = CCMenu::createWithItem(pReturnMenuItem);
    addChild(pMenu);
    pReturnMenuItem->setScale(0.5f);
    pReturnMenuItem->setRotation(-90);
    pMenu->setPosition(ccp(winSize.width - 50.0f, 50.f));

    return true;
}

cocos2d::CCScene* StoryLayer::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        StoryLayer *layer = StoryLayer::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}