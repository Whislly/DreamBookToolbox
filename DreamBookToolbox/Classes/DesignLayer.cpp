#include "DesignLayer.h"
#include "DBActionSprite.h"
#include "CommonHelper.h"

USING_NS_CC;

void DesignLayer::addTimeLabel()
{
    CCSize contentSize = this->getContentSize();
    m_lblTime = CCLabelBMFont::create("Time: 0", "font09.fnt");
    m_lblTime->setAnchorPoint(ccp(1, 1));
    this->addChild(m_lblTime, -1);
    m_lblTime->setPosition(ccp(contentSize.width * 0.5f - 20.0f, contentSize.height - 2.0f));
    m_lblTime->retain();
}

DesignLayer* DesignLayer::create( const cocos2d::ccColor4B& color )
{
    DesignLayer* pLayer = new DesignLayer();
    pLayer->initWithColor(color);
    pLayer->autorelease();
    return pLayer;
}

DesignLayer::DesignLayer()
    : m_lblTime(NULL)
    , m_time(0.0f)
    , m_currentSpriteTag(-1)
    , m_finishedActionCount(1)
    , m_enlarge(NULL)
    , m_reduce(NULL)
    , m_currentSprite(NULL)
    , m_input(NULL)
{
    m_enlarge = CCSpriteEx::create("enhance.png");
    addChild(m_enlarge, 5);
    m_enlarge->setScale(0.3f);
    m_enlarge->setSelectorForSingleClick(this, menu_selector(DesignLayer::enlarge));
    m_enlarge->setVisible(false);
    //m_enlarge->setPosition(ccp(300, 300));

    m_reduce = CCSpriteEx::create("small.png");
    addChild(m_reduce, 5);
    m_reduce->setScale(0.3f);
    //m_reduce->setPosition(ccp(300, 300));
    m_reduce->setSelectorForSingleClick(this, menu_selector(DesignLayer::reduce));
    m_reduce->setVisible(false);

    m_input = PropertyInput::create();
    //sprite - 1
    m_input->sprite = CCSprite::create("Images/PhysicsTech/Prop_Quality.png");
    m_input->sprite->setAnchorPoint(cocos2d::CCPointZero);
    m_input->sprite->setPosition(ccp(0, 100));
    m_input->addChild(m_input->sprite);
    //input - sprite - 1
    m_input->inputSprite = CCSprite::create("Images/PhysicsTech/QualityInput.png");
    m_input->inputSprite->setAnchorPoint(cocos2d::CCPointZero);
    m_input->inputSprite->setPosition(ccp(m_input->sprite->getPositionX() + 32 + 5, 95));
    m_input->addChild(m_input->inputSprite);
    //input - 1
    m_input->text->setPosition(ccp(m_input->sprite->getPositionX() + 32 + 25, 105));
    m_input->inputRect = CCSize(100, 20);
    m_input->text->setContentSize(m_input->inputRect);
    this->addChild(m_input);
    m_input->setVisible(false);
}

DesignLayer::~DesignLayer()
{
    if(m_lblTime)
    {
        m_lblTime->release();
    }
    m_lblTime = NULL;
}

void DesignLayer::addSpriteEx( cocos2d::CCSpriteEx* pSprite )
{
    pSprite->setSelectorForDoubleClick(this, menu_selector(DesignLayer::showScaleToolButtons));
    this->addChild(pSprite, 0, pSprite->getTag());
}

void DesignLayer::addSpriteEx( cocos2d::CCSpriteEx* pSprite, int zOrder )
{
    pSprite->setSelectorForDoubleClick(this, menu_selector(DesignLayer::showScaleToolButtons));
    this->addChild(pSprite, zOrder, pSprite->getTag());
}

void DesignLayer::removeSpriteEx( int tag, bool cleanup )
{
    this->removeChildByTag(tag, cleanup);
}

bool DesignLayer::isContainsChild( int tag )
{
    CCNode* pChild = this->getChildByTag(tag);
    return (pChild != NULL);
}

void DesignLayer::stopChildrenActions( int tag )
{
    for (int i = tag; i >= 0; i--)
    {
        DBActionSprite* pActionSprite = (DBActionSprite*)(this->getChildByTag(i));
        if (pActionSprite)
        {
            pActionSprite->setEnableTime(false);
        }
    }
}

void DesignLayer::focusChild( cocos2d::CCSprite* pSprite )
{
    float scaleValue = pSprite->getScale();
    CCActionInterval* pAction = CCRepeatForever::create(
        (CCActionInterval*)CCSequence::create(CCScaleTo::create(0.4f, scaleValue * 1.1f), 
        CCScaleTo::create(0.4f, scaleValue * 0.9f), NULL)
        );
    pSprite->runAction(pAction);
    pAction->setTag(FocusActionTag);
}

void DesignLayer::designSpriteStatus( int tag )
{
    setEnableTime(false);
    DBActionSprite* pActionSprite = NULL;
    if(m_currentSpriteTag != -1)
    {
        pActionSprite = (DBActionSprite*)(this->getChildByTag(m_currentSpriteTag));
        if (pActionSprite)
        {
            //pActionSprite->setScaleEnabled(false);
        }
    }
    m_currentSpriteTag = tag;

    this->stopChildrenActions(tag);
    pActionSprite = (DBActionSprite*)(this->getChildByTag(tag));
    pActionSprite->designStatus(m_time);
    //pActionSprite->setScaleEnabled(true);
    this->focusChild(pActionSprite);
}   

void DesignLayer::runChildrenActions( int tag )
{
    for (int i = tag - 1; i >= 0; i--)
    {
        DBActionSprite* pActionSprite = (DBActionSprite*)(this->getChildByTag(i));
        if (pActionSprite)
        {
            pActionSprite->excuteAction(this->m_time);
        }
    }
}

void DesignLayer::saveData()
{
    CCObject* pObj = NULL;
    CCArray* pChildren = getChildren();

    char key[255] = {0};
    char content[255] = {0};
    int idx = 0;
    CCUserDefault* pUserData = CCUserDefault::sharedUserDefault();
    CCARRAY_FOREACH(pChildren, pObj)
    {
        CCNode* pNode = (CCNode*)pObj;
        if (pNode->getTag() == -1)
        {
            continue;
        }
        sprintf(key, "tagArray%d", idx);
        sprintf(content, "%s,%d", content, pNode->getTag());
        if (strlen(content + 1) >= 248)
        {
            pUserData->setStringForKey(key, content + 1);
            idx++;
            memset(content, 0, 255);
        }
    }
    if (strlen(content + 1) > 0)
    {
        pUserData->setStringForKey(key, content + 1);
    }

    CCARRAY_FOREACH(pChildren, pObj)
    {
        DBActionSprite* pActionSprite = (DBActionSprite*)(pObj);
        if (pActionSprite && (pActionSprite->getTag() != -1))
        {
            pActionSprite->save();
        }
    }
}

void DesignLayer::loadData()
{
    CCUserDefault* pUserData = CCUserDefault::sharedUserDefault();
    int idx = 0;
    char key[255] = {0};
    sprintf(key, "tagArray%d", idx);
    char content[255] = {0};
    sprintf(content, "%s", pUserData->getStringForKey(key).c_str());
    CCArray* pIntArray = NULL;
    while(strlen(content) > 0)
    {
        if (!pIntArray)
        {
            pIntArray = CommonHelper::getIntArray(content);
        }
        else
        {
            CCArray* anotherIntArray = CommonHelper::getIntArray(content);
            if (anotherIntArray)
            {
                pIntArray->addObjectsFromArray(anotherIntArray);
            }
        }

        if (strlen(content) < 248)
        {
            break;
        }
        else
        {
            idx++;
            sprintf(key, "tagArray%d", idx);
            sprintf(content, "%s", pUserData->getStringForKey(key).c_str());
        }
    }

    CCObject* pObj = NULL;
    CCARRAY_FOREACH(pIntArray, pObj)
    {
        CCInteger* pInt = (CCInteger*)pObj;
        DBActionSprite* pActionSprite = DBActionSprite::create();
        pActionSprite->setTag(pInt->getValue());
        pActionSprite->load();
        addSpriteEx(pActionSprite);
        pActionSprite->readStatus(0.0f);
    }
}

void DesignLayer::addFinishedActionCount()
{
    this->m_finishedActionCount++;
}

void DesignLayer::runChildrenActions()
{
    DBActionSprite* pActionSprite = NULL;
    if(m_currentSpriteTag != -1)
    {
        pActionSprite = (DBActionSprite*)(this->getChildByTag(m_currentSpriteTag));
        if (pActionSprite)
        {
            //pActionSprite->setScaleEnabled(false);
        }
    }
    m_currentSpriteTag = -1;
    m_lblTime->setString("Time: 0");
    m_time = 0.0f;
    setEnableTime(true);

    for (int i = 0; i < getChildrenCount(); i++)
    {
        DBActionSprite* pActionSprite = (DBActionSprite*)(this->getChildByTag(i));
        if (pActionSprite)
        {
            pActionSprite->excuteAction(this->m_time, this, callfunc_selector(DesignLayer::addFinishedActionCount));
        }
    }
}

void DesignLayer::activeActions( int tag )
{
    DBActionSprite* pActionSprite = NULL;
    if(m_currentSpriteTag != -1)
    {
        pActionSprite = (DBActionSprite*)(this->getChildByTag(m_currentSpriteTag));
        if (pActionSprite)
        {
            //pActionSprite->setScaleEnabled(false);
        }
    }
    m_currentSpriteTag = tag;
    m_lblTime->setString("Time: 0");
    m_time = 0.0f;

    pActionSprite = (DBActionSprite*)(this->getChildByTag(tag));
    if (pActionSprite)
    {
        /*m_time = pActionSprite->getStartTime();
        char time_str[20];
        sprintf(time_str, "Time: %.1f", this->m_time);
        m_lblTime->setString(time_str);*/
        pActionSprite->clearAction(m_time);
        pActionSprite->designAction(this->m_time);
        this->focusChild(pActionSprite);
        //pActionSprite->setScaleEnabled(true);
    }
    this->runChildrenActions(tag);
    setEnableTime(true);
}

void DesignLayer::step( float time )
{
    int temp = (int)(time * 10);
    this->m_time += (temp * 0.1);

    /*if (this->m_finishedActionCount == getChildrenCount())
    {
    setEnableTime(false);
    m_finishedActionCount = 1;
    }*/

    CCLOG("DesignLayer::step(%f)", m_time);

    char time_str[20];
    sprintf(time_str, "Time: %.1f", this->m_time);
    m_lblTime->setString(time_str);
}

void DesignLayer::setEnableTime( bool isTime )
{
    if (isTime)
    {
        this->schedule(schedule_selector(DesignLayer::step), 0.1f);
    }
    else
    {
        this->unschedule(schedule_selector(DesignLayer::step));
    }
}

void DesignLayer::enlarge( cocos2d::CCObject* pSender )
{
    if (!m_currentSprite)
    {
        return;
    }
    float scale = m_currentSprite->getScale();
    m_currentSprite->setScale(scale * 1.1f);
}

void DesignLayer::reduce( cocos2d::CCObject* pSender )
{
    if (!m_currentSprite)
    {
        return;
    }
    float scale = m_currentSprite->getScale();
    m_currentSprite->setScale(scale * 0.9f);
}

void DesignLayer::showScaleToolButtons( cocos2d::CCObject* pSender )
{
    DBActionSprite* pSprite = (DBActionSprite*)pSender;
    CCPoint pos = pSprite->getPosition();
    m_enlarge->setVisible(true);
    m_reduce->setVisible(true);
    //m_input->setVisible(true);

    m_enlarge->setPosition(ccpAdd(pos, ccp(40.0f, 0.0f)));
    m_enlarge->runAction(CCShow::create());
    m_reduce->setPosition(ccpSub(pos, ccp(40.0f, 0.0f)));
    m_reduce->runAction(CCShow::create());

    /*m_input->setPosition(ccpSub(pos, ccp(0.0f, 40.0f)));
    m_input->runAction(CCShow::create());*/

    m_currentSprite = pSprite;
}

bool DesignLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    CCPoint touchLocation = pTouch->getLocation();
    if (!m_reduce->boundingBox().containsPoint(touchLocation) && 
        !m_enlarge->boundingBox().containsPoint(touchLocation) &&
        !m_input->boundingBox().containsPoint(touchLocation) &&
        m_currentSprite && (!m_currentSprite->boundingBox().containsPoint(touchLocation)))
    {
        m_enlarge->setVisible(false);
        m_reduce->setVisible(false);
        m_input->setVisible(false);
        const char* content = m_input->text->getString();
        if (content && strlen(content) > 0)
        {
            m_currentSprite->setInputContent(content, m_time);
            m_currentSprite->setSelectorForInput(this, menu_selector(DesignLayer::waitInput));
        }
    }
    return false;
}

void DesignLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0/*priority=??*/, false);
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCDirector::sharedDirector()->setVoiceRecognitionTarget(this, voiceRecognition_selector(DesignLayer::voiceRecognition));
    #endif
}

void DesignLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCDirector::sharedDirector()->setVoiceRecognitionTarget(NULL, NULL);
    #endif
    CCLayerColor::onExit();
}

void DesignLayer::voiceRecognition( const char* voiceContent )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (m_currentSprite)
    {
        m_currentSprite->checkInputContent(voiceContent);
    }
#endif
}

void DesignLayer::voiceRecognition( cocos2d::CCObject* pSender )
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
    m_input->setSelectorForLeave(NULL, NULL);
    if (m_currentSprite)
    {
        m_currentSprite->checkInputContent(m_input->text->getString());
    }
#endif
}

void DesignLayer::waitInput( cocos2d::CCObject* pSender )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    this->activeVoiceService();
#else
    DBActionSprite* pSprite = (DBActionSprite*)pSender;
    CCPoint pos = pSprite->getPosition();
    m_input->setVisible(true);
    m_input->runAction(CCShow::create());
    m_input->setPosition(ccpSub(pos, ccp(0.0f, 40.0f)));
    m_input->setSelectorForLeave(this, menu_selector(DesignLayer::voiceRecognition));
#endif
}
