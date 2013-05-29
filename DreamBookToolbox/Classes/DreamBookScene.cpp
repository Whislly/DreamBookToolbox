#include "DreamBookScene.h"
#include "CCSpriteEx.h"
#include "GestureLayer.h"
#include "ObserveLayer.h"
#include "DBActionSprite.h"
#include "MainScene.h"
#include "Define.h"
#include "CCSpriteEx.h"
#include "FileSprite.h"
#include "FrameSprite.h"

using namespace cocos2d;
using namespace cocos2d::extension;

#define USER_DEFAULT_NAME "UserDefault.xml"

cocos2d::CCParticleSystem* DreamBookLayer::createGestureStyle()
{
	cocos2d::CCParticleSystem* particle = CCParticleSun::create();
	particle->setTexture( CCTextureCache::sharedTextureCache()->addImage("fire.png") );
	return particle;
}

void DreamBookLayer::gestureResult( const char* name, double score )
{
    if (strcmp(name, "Circle") == 0)
    {
        m_designLayer->runChildrenActions();
    }
    /*else if(strcmp(name, "CheckMark") == 0)
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
        newSprite->addResourcePath(clickCell->getResourceFileArray());
        m_designLayer->addSpriteEx(newSprite);

        CCSize contentSize = m_designLayer->getContentSize();
        CCPoint position = ccp(contentSize.width * RandomFloat(0.01f, 1.0f), contentSize.height * RandomFloat(0.01f, 1.0f));
        newSprite->setPosition(position);

        //////////////////////////////////////////////////////////////////////////
        ///Fixed the issue of copies for Actions
        ///[Biao Feng] Start
        //////////////////////////////////////////////////////////////////////////
        cocos2d::CCArray *arr = clickCell->getAllActions();
        if (arr && arr->count() > 0)
        {
            for (int j = 0; j < arr->count(); j++)
            {
                cocos2d::CCAction *act = (cocos2d::CCAction*)arr->objectAtIndex(j);
                if (act->getTag() == FocusActionTag)
                {
                    continue;
                }
                newSprite->runAction(act);
            }
        }
        //////////////////////////////////////////////////////////////////////////
        ///Fixed the issue of copies for Actions
        ///[Biao Feng] End
        //////////////////////////////////////////////////////////////////////////
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
            pCell->setSelectorForSingleClick(this, menu_selector(DreamBookLayer::activeCell));
            pCell->setSelectorForDoubleClick(this, menu_selector(DreamBookLayer::activeActions));
            const char* information = pSprite->objectInfor();
            if (strstr(information, "FileSprite"))
            {
                pCell->addResourcePath(((FileSprite*)pSprite)->path);
            }
            else if(strstr(information, "FrameSprite"))
            {
                pCell->addResourcePath(((FrameSprite*)pSprite)->pathArray);
            }
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
    pDefaultCell->setSelectorForSingleClick(this, menu_selector(DreamBookLayer::showPicPickupLayer));
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

void DreamBookLayer::saveDataToCloud( CCObject* pSender )
{
    CCLabelTTF* lbDebug = CCLabelTTF::create("Start upload...", "Arial", 40);
    lbDebug->setColor(ccWHITE);
    addChild(lbDebug, 1, 99);
    lbDebug->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width * 0.5f, CCDirector::sharedDirector()->getWinSize().height * 0.5f));
    lbDebug->runAction(CCRepeatForever::create(CCBlink::create(1.0f, 1)));
    m_designLayer->saveData();
	this->file->uploadFile(CCFileUtils::sharedFileUtils()->getWritablePath().c_str(), USER_DEFAULT_NAME);
	this->file->deleteFile(this->object->Get<const char*>("name"));
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
        pBackItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 50, 25));

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

        this->readyUploadFile();

        CCSpriteEx* saveButton = CCSpriteEx::create("save.png");
        saveButton->setSelectorForSingleClick(this, menu_selector(DreamBookLayer::saveDataToCloud));
        saveButton->setScale(0.5f);
        addChild(saveButton);
        saveButton->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 130, 30));

        this->DownloadFile();

        bRet = true;
    } while (0);

    return bRet;
}

DreamBookLayer::DreamBookLayer()
    : m_designLayer(NULL)
    , m_observeLayer(NULL)
    , m_picPickupLayer(NULL)
    , file(NULL)
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
    if (file)
    {
        file->release();
    }
	if (this->object)
    {
        this->object->release();
    }
}

void DreamBookLayer::UploadCompleted( FileInfo* fileInfo, ParseError* error )
{
    removeChildByTag(99, true);
    if (fileInfo)
    {
        object->Set("url", fileInfo->GetUrl().c_str());
        object->Set("name", fileInfo->GetFileName().c_str());
        object->save();
    }
}

void DreamBookLayer::readyUploadFile()
{
    /*this->lbDebug = CCLabelTTF::create("Start", "Arial", 24);
    this->lbDebug->setColor(ccc3(255, 255, 1));
    this->lbDebug->setPosition(ccp(0, winSize.height - 100));
    this->lbDebug->setAnchorPoint(CCPointZero);
    this->addChild(this->lbDebug);*/

	//add Whislly test start
	cocos2d::extension::Parse parse;
	parse.setApplicationId("n1s82tGoQDgDM09qFNm0UQkKNO7yW1gqpQSavT5n");
	parse.setApiKey("Nh4K6HM2tohkmQdeu5vfN7ZWP83OMQ72uH0YMtW7");
	parse.setMasterKey("doMxP88XvGkICRrI3gPcCdwWMI26QZfzPMKER33m");

	file = new ParseFile();
	file->deleteFileCompleted.Set(this, (Delegate<bool, ParseError*>::MemberFun)&DreamBookLayer::DeleteCompleted);
	file->uploadFileCompleted.Set(this, (Delegate<FileInfo*, ParseError*>::MemberFun)&DreamBookLayer::UploadCompleted);
	file->downloadFileCompleted.Set(this, (Delegate<const char*, ParseError*>::MemberFun)&DreamBookLayer::DownloadCompleted);
}

void DreamBookLayer::DeleteCompleted( bool isSuccess, cocos2d::extension::ParseError* error )
{
	if (isSuccess)
	{
		
	}
    removeChildByTag(98, true);
}

void DreamBookLayer::FindCompleted( cocos2d::CCArray* array, cocos2d::extension::ParseError* error )
{
    if (array && array->count() > 0)
    {
        this->object = (ParseObject*)array->objectAtIndex(0);
		this->object->retain();

        const char* url = this->object->Get<const char*>("url");
        std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath() + USER_DEFAULT_NAME;
        this->file->downloadFile(url, filePath.c_str());
        array->removeAllObjects();
    }
	else
	{
		this->object = new ParseObject("DataFile");
		this->object->Add("url", "");
		this->object->Add("name", "");
	}

    removeChildByTag(98, true);
}

void DreamBookLayer::loadDataToObserveLayer()
{
    CCArray* pChildren = m_designLayer->getChildren();
    for (int i = 0; i < m_designLayer->getChildrenCount(); i++)
    {
        DBActionSprite* pSprite = (DBActionSprite*)(pChildren->objectAtIndex(i));
        if (pSprite && !m_observeLayer->getChildByTag(pSprite->getTag()))
        {
            CCSpriteEx* pCell = CCSpriteEx::createWithTexture(pSprite->getTexture(), pSprite->getTextureRect());
            pCell->setSelectorForSingleClick(this, menu_selector(DreamBookLayer::activeCell));
            pCell->setSelectorForDoubleClick(this, menu_selector(DreamBookLayer::activeActions));
            pCell->addResourcePath(pSprite->getResourceFileArray());
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

void DreamBookLayer::DownloadCompleted( const char* path, cocos2d::extension::ParseError* error )
{
    if (path)
    {
        m_designLayer->loadData();
        loadDataToObserveLayer();
    }

    removeChildByTag(98, true);
}

void DreamBookLayer::DownloadFile()
{
    CCLabelTTF* lbDebug = CCLabelTTF::create("Download check...", "Arial", 40);
    lbDebug->setColor(ccWHITE);
    addChild(lbDebug, 1, 98);
    lbDebug->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width * 0.5f, CCDirector::sharedDirector()->getWinSize().height * 0.5f));
    lbDebug->runAction(CCRepeatForever::create(CCBlink::create(1.0f, 1)));

    cocos2d::extension::ParseQuery* query = new cocos2d::extension::ParseQuery("DataFile");
    query->findObjectsCompleted.Set(this, (Delegate<CCArray*, ParseError*>::MemberFun)&DreamBookLayer::FindCompleted);
    query->findObjects();
    query->release();
}