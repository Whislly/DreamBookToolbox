#include "DreamBookScene.h"
#include "CCSpriteEx.h"
#include "GestureLayer.h"
#include "ObserveLayer.h"
#include "DBActionSprite.h"
#include "MainScene.h"

using namespace cocos2d;

cocos2d::CCParticleSystem* DreamBookLayer::createGestureStyle()
{
	cocos2d::CCParticleSystem* particle = CCParticleSun::create();
	particle->setTexture( CCTextureCache::sharedTextureCache()->addImage("fire.png") );
	return particle;
}

void DreamBookLayer::gestureResult( const char* name, double score )
{
    /*CCLabelTTF* lblTest = (CCLabelTTF*)(((CCLayer*)pTarget)->getChildByTag(2));
    if (strcmp(name, "Circle") == 0)
    {
    lblTest->setString("Circle");
    }
    else if(strcmp(name, "CheckMark") == 0)
    {
    lblTest->setString("CheckMark");
    }
    else
    {
    lblTest->setString("None.");
    }*/
}

CCScene* DreamBookLayer::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        DreamBookLayer *layer = DreamBookLayer::create();
        CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);

		GestureLayer* gestureLayer = GestureLayer::create(layer->createGestureStyle());
		gestureLayer->setGestureHandler(layer, gesture_selector(DreamBookLayer::gestureResult));
		CC_BREAK_IF(! gestureLayer);

        // add layer as a child to scene
        scene->addChild(gestureLayer);
    } while (0);

    // return the scene
    return scene;
}

void DreamBookLayer::menuBackCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(0.5f, MainLayer::scene(), true));
}

void DreamBookLayer::addBackground()
{
    CCSprite* pBackground = CCSprite::createWithSpriteFrameName("Back.png");
    this->addChild(pBackground);
    CCSize backSize = pBackground->getContentSize();
    pBackground->setPosition(ccp(backSize.width * 0.4f, backSize.height * 0.5f - 70.0f));
}

void DreamBookLayer::activeActions( CCObject* pSender )
{
    CCSpriteEx* clickCell = (CCSpriteEx*)pSender;
    int tag = clickCell->getTag();
    m_designLayer->activeActions(tag);
}

void DreamBookLayer::activeCell( CCObject* pSender )
{
    CCSpriteEx* clickCell = (CCSpriteEx*)pSender;
    int tag = clickCell->getTag();
    if (clickCell)
    {
        m_observeLayer->highlightColor(tag, ccWHITE);
    }
    if (!m_designLayer->isContainsChild(tag))
    {
        DBActionSprite* newSprite = DBActionSprite::createWithTexture(clickCell->getTexture(), clickCell->getTextureRect(), clickCell->getTag());
        m_designLayer->addSpriteEx(newSprite);

        CCSize contentSize = m_designLayer->getContentSize();
        CCPoint position = ccp(contentSize.width * RandomFloat(0.01f, 1.0f), contentSize.height * RandomFloat(0.01f, 1.0f));
        newSprite->setPosition(position);
    }
    m_designLayer->designSpriteStatus(tag);
}

void DreamBookLayer::addNewCell( cocos2d::CCObject* pSender, cocos2d::CCArray* pElementArray )
{
    CCObject* pObj = NULL;
    int count = pElementArray->count();
    for (int i = 0; i < count; i++)
    {
        CCSprite* pSprite = (CCSprite*)pElementArray->objectAtIndex(i);
        if(pSprite)
        {
            CCSpriteEx* pCell = CCSpriteEx::createWithTexture(pSprite->getTexture(), pSprite->getTextureRect());
            pCell->setSelectorWithTarget(this, menu_selector(DreamBookLayer::activeCell), menu_selector(DreamBookLayer::activeActions), NULL);
            m_observeLayer->addCell(pCell);

            cocos2d::CCArray *arr = pSprite->getAllActions();
            if (arr && arr->count() > 0)
            {
                for (int j = 0; j < arr->count(); j++)
                {
                    cocos2d::CCAction *act = (cocos2d::CCAction*)arr->objectAtIndex(j);
                    pCell->runAction(act);
                }
                //CCPoint position = pCell->getPosition();
                //pCell->setPosition(ccp(position.x - 15, position.y - 15));
            }
        }
    }
}

void DreamBookLayer::showPicPickupLayer( CCObject* pSender )
{
    CCDirector::sharedDirector()->pushScene(PicPickupLayer::scene(this, importElement_selector(DreamBookLayer::addNewCell)));
}

cocos2d::CCSprite* DreamBookLayer::getDefaultCell()
{
    CCSpriteEx* pDefaultCell = CCSpriteEx::createWithSpriteFrameName("Add.png");
    pDefaultCell->setSelectorWithTarget(this, menu_selector(DreamBookLayer::showPicPickupLayer), NULL, NULL);
    CCActionInterval* pCamerAction = CCOrbitCamera::create(0.5f, 1, 0, 0, 30, 0, 0);
    CCActionInterval* pAction = (CCActionInterval*)CCSequence::create(CCDelayTime::create(1.5f), pCamerAction, pCamerAction->reverse(), NULL);
    pDefaultCell->runAction(CCRepeatForever::create(pAction));
    return pDefaultCell;
}

void DreamBookLayer::addObserveLayer()
{
    m_observeLayer = ObserveLayer::create(ccc4(ccBLACK.r, ccBLACK.g, ccBLACK.b, 125));
    this->addChild(m_observeLayer, 1);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize size = CCSize(winSize.width * 0.05f, winSize.height * 0.95f);
    m_observeLayer->setContentSize(size);
    m_observeLayer->setOrientation(kOrientationVer);
    m_observeLayer->loadDefaultCell(this->getDefaultCell());
    m_observeLayer->setPosition(ccp(5, winSize.height - size.height - 20.0f));
    m_observeLayer->setAnchorPoint(CCPointZero);
    m_observeLayer->retain();
}

void DreamBookLayer::addDesignLayer()
{
    m_designLayer = DesignLayer::create(ccc4(ccBLACK.r, ccBLACK.g, ccBLACK.b, 50));
    this->addChild(m_designLayer);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize size = CCSize(winSize.width * 0.95f - 20, winSize.height * 0.95f);
    m_designLayer->setContentSize(size);
    m_designLayer->setPosition(ccp(winSize.width * 0.05f + 15, winSize.height - size.height - 20.0f));
    m_designLayer->setAnchorPoint(CCPointZero);
    m_designLayer->retain();
    m_designLayer->addTimeLabel();
}

// on "init" you need to initialize your instance
bool DreamBookLayer::init()
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

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemSprite *pBackItem = CCMenuItemSprite::create(
            CCSprite::createWithSpriteFrameName("b1.png"), CCSprite::createWithSpriteFrameName("b2.png"), this,
            menu_selector(DreamBookLayer::menuBackCallback));
        CC_BREAK_IF(! pBackItem);

        // Place the menu item bottom-right conner.
        pBackItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 50, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pBackItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        /*CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        for (int i = 0; i < 10; i++)
        {
        CCSpriteEx* pSprite = CCSpriteEx::create("CloseNormal.png");
        CCPoint position = ccp(winSize.width * RandomFloat(0.01f, 1.0f), winSize.height * RandomFloat(0.01f, 1.0f));
        addChild(pSprite);
        pSprite->setPosition(position);        
        pSprite->setSelectorWithTarget(this, menu_selector(MainLayer::addNewCell), menu_selector(MainLayer::addNewCell), menu_selector(MainLayer::addNewCell));
        }*/

        this->addBackground();
        this->addObserveLayer();
        this->addDesignLayer();

        bRet = true;
    } while (0);

    return bRet;
}

DreamBookLayer::DreamBookLayer()
    : m_designLayer(NULL)
    , m_observeLayer(NULL)
    , m_picPickupLayer(NULL)
{

}

DreamBookLayer::~DreamBookLayer()
{
    if(m_observeLayer)
    {
        m_observeLayer->release();
    }
    if (m_designLayer)
    {
        m_designLayer->release();
    }
    if (m_picPickupLayer)
    {
        m_picPickupLayer->release();
    }
}