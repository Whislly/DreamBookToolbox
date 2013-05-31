#include "DBActionSprite.h"
#include "PropertyLayer.h"

USING_NS_CC;

DBActionSprite::DBActionSprite()
    : m_time(0.0f)
    , m_status(kDesignStatus)
    , m_data(NULL)
    , m_startTime(0.0f)
    , m_endTime(0.0f)
    , m_voiceContent(NULL)
    , m_finishedActionListener(NULL)
    , m_finishedActionSelector(NULL)
    , m_isBreak(false)
    , m_inputListener(NULL)
    , m_inputSelector(NULL)
{
    m_data = DBData::create();
    m_data->retain();
    m_data->m_resourceFileArray = this->m_resourceFileArray;
}

DBActionSprite::~DBActionSprite()
{
    if(m_data)
    {
        m_data->release();
    }
    m_data = NULL;
    CCSpriteEx::~CCSpriteEx();
}

DBActionSprite* DBActionSprite::createWithTexture( cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect )
{
    DBActionSprite* pActionSprite = new DBActionSprite();
    if (pActionSprite && pActionSprite->initWithTexture(pTexture, rect))
    {
        pActionSprite->autorelease();
        /*pActionSprite->m_data = DBData::create();
        pActionSprite->m_data->retain();*/
        return pActionSprite;
    }
    CC_SAFE_DELETE(pActionSprite);
    return NULL;
}

DBActionSprite* DBActionSprite::createWithTexture( cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect, int tag )
{
    DBActionSprite* pActionSprite = new DBActionSprite();
    if (pActionSprite && pActionSprite->initWithTexture(pTexture, rect))
    {
        pActionSprite->autorelease();
        pActionSprite->setTag(tag);
        return pActionSprite;
    }
    CC_SAFE_DELETE(pActionSprite);
    return NULL;
}

bool DBActionSprite::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    stopActionByTag(FocusActionTag);
    //setScale(1.0f);
    return CCSpriteEx::ccTouchBegan(pTouch, pEvent);
}

void DBActionSprite::touchMovedDesign(CCTouch *pTouch)
{
    CCPoint touchLocation = pTouch->getLocation();
    CCPoint pos = ccpSub(touchLocation, this->getParent()->getPosition());
    writeStatus(pos, this->m_time);
}

void DBActionSprite::touchMovedAction( cocos2d::CCTouch *pTouch )
{
    CCPoint touchLocation = pTouch->getLocation();
    CCPoint pos = ccpSub(touchLocation, this->getParent()->getPosition());
    writeStatus(pos, this->m_time);
}

void DBActionSprite::writeStatus( CCPoint pos, float time )
{
    this->setPosition(pos);
    DBPropertyData* propertyData = m_data->getDBPropertyData(this->m_time);
    propertyData->setPosition(pos);
    propertyData->setScale(getScale());
}

void DBActionSprite::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
    CCSpriteEx::ccTouchMoved(pTouch, pEvent);
    switch(m_status)
    {
    case kDesignStatus:
        touchMovedDesign(pTouch);
        break;
    case kDesignAction:
        touchMovedAction(pTouch);
        break;
    default:
        break;
    }
}

void DBActionSprite::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
    CCSpriteEx::ccTouchEnded(pTouch, pEvent);
}

void DBActionSprite::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
    CCSpriteEx::ccTouchCancelled(pTouch, pEvent);
}

void DBActionSprite::readStatus( float time )
{
    DBPropertyData* propertyData = m_data->getDBPropertyData(time);
    CCPoint pos = propertyData->getPosition();
    if ((pos.x != -999) && (pos.y != -999))
    {
        this->setPosition(pos);
    }
    else
    {
        this->writeStatus(this->getPosition(), time);
    }
    setScale(propertyData->getScale());
}

void DBActionSprite::designStatus(float time)
{
    m_startTime = m_startTime < time ? m_startTime : time;
    m_time = time;
    m_status = kDesignStatus;
    setEnableTime(false);
    readStatus(time);
}

void DBActionSprite::clearAction( float time )
{
    this->m_data->removeDBPropertyData(time, m_endTime);
}

void DBActionSprite::designAction(float time)
{
    this->readStatus(time);
    m_status = kDesignAction;
    this->m_time = time;
    setEnableTime(true);
}

void DBActionSprite::excuteAction( float time )
{
    this->readStatus(time);
    m_status = kExcuteAction;
    this->m_time = time;
    setEnableTime(true);
}

void DBActionSprite::excuteAction( float time, cocos2d::CCObject *target, cocos2d::SEL_CallFunc selector )
{
    if (target && selector)
    {
        m_finishedActionListener = target;
        m_finishedActionSelector = selector;
        this->excuteAction(time);
    }
}

void DBActionSprite::save(rapidjson::PrettyWriter<rapidjson::FileStream>& write)
{
	write.StartObject();

	write.String("Tag");
	write.Int(this->getTag());

    this->m_data->save(write);

	write.EndObject();
}

void DBActionSprite::transform( float time )
{
    DBPropertyData* propertyData = m_data->getDBPropertyData(time);
    CCPoint pos = propertyData->getPosition();
    if ((pos.x != -999) && (pos.y != -999))
    {
        this->runAction(CCSpawn::create(CCMoveTo::create(0.1f, pos), CCScaleTo::create(0.1f, propertyData->getScale()), NULL));
    }
}

void DBActionSprite::step( float time )
{
    if (m_isBreak)
    {
        return;
    }

    if (this->m_data->getDBPropertyData(m_time)->getInputContent())
    {
        m_isBreak = true;
        if (m_inputSelector && m_inputListener)
        {
            (m_inputListener->*m_inputSelector)(this);
        }
    }

    int temp = (int)(time * 10);
    this->m_time += (temp * 0.1);

    /*if (!this->m_data->getEndTime() <= this->m_time)
    {
    setEnableTime(false);
    if (m_finishedActionListener && m_finishedActionSelector)
    {
    (m_finishedActionListener->*m_finishedActionSelector)();
    }
    }*/

    switch(m_status)
    {
    case kExcuteAction:
        this->transform(this->m_time);
        break;
    default:
        break;
    }
}

void DBActionSprite::setEnableTime( bool isTime )
{
    if (isTime)
    {
        this->schedule(schedule_selector(DBActionSprite::step), 0.1f);
    }
    else
    {
        m_endTime = m_time;
        this->unschedule(schedule_selector(DBActionSprite::step));
    }
}

void DBActionSprite::load(rapidjson::Value& value)
{
    this->m_data->load(value);
    int count = m_data->m_resourceFileArray->count();
    if (count > 0)
    {
        CCArray* resourceArray = m_data->m_resourceFileArray;
        this->initWithFile(((CCString*)resourceArray->objectAtIndex(0))->getCString());

        if (count > 1)
        {
            CCObject* pObj = NULL;
            CCArray* moreFrames = CCArray::createWithCapacity(count);
            CCARRAY_FOREACH(resourceArray, pObj)
            {
                CCString* path = (CCString*)pObj;
                CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(path->getCString());
                if (pTexture)
                {
                    CCRect rect = CCRectZero;
                    rect.size = pTexture->getContentSize();
                    CCSpriteFrame* pFrame = CCSpriteFrame::create(path->getCString(), rect);
                    moreFrames->addObject(pFrame);
                }
            }
            CCAnimation* animation = CCAnimation::createWithSpriteFrames(moreFrames, 0.25f);
            this->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
        }
    }
}

DBActionSprite* DBActionSprite::create()
{
    DBActionSprite* pActionSprite = new DBActionSprite();
    if (pActionSprite)
    {
        pActionSprite->autorelease();
        /*pActionSprite->m_data = DBData::create();
        pActionSprite->m_data->retain();*/
        return pActionSprite;
    }
    CC_SAFE_DELETE(pActionSprite);
    return NULL;
}

void DBActionSprite::setInputContent( const char* inputContent, float time )
{
    if (m_status == kDesignStatus)
    {
        m_data->getDBPropertyData(time)->setInputContent(inputContent);
    }
}

void DBActionSprite::checkInputContent( const char* inputContent )
{
    CCString* correctContent = m_data->getDBPropertyData(m_time)->getInputContent();
    if (strstr(inputContent, correctContent->getCString()))
    {
        m_isBreak = false;
    }
    else
    {
        if (m_inputSelector && m_inputListener)
        {
            (m_inputListener->*m_inputSelector)(this);
        }
    }
}

void DBActionSprite::setSelectorForInput( CCObject *target, SEL_MenuHandler inputSelector )
{
    m_inputListener = target;
    m_inputSelector = inputSelector;
}
