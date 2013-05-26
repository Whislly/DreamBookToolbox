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
        pActionSprite->m_data = DBData::create();
        pActionSprite->m_data->retain();
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

void DBActionSprite::save()
{
    this->m_data->save(this->getTag());
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

void DBActionSprite::setVoiceEnabeld( bool isEnabled )
{
    if (isEnabled)
    {
        setSelectorForLongClick(this, menu_selector(DBActionSprite::PressDelayEvent));
    }
    else
    {
        setSelectorForLongClick(NULL, NULL);
    }
}

void DBActionSprite::PressDelayEvent(CCObject* pSender)
{
	PropertyInput *input1 = PropertyInput::create();

	//sprite - 1
	input1->sprite = CCSprite::create("Images/PhysicsTech/Prop_Quality.png");
	input1->sprite->setAnchorPoint(cocos2d::CCPointZero);
	input1->sprite->setPosition(ccp(0, 100));
	input1->addChild(input1->sprite);
	//input - sprite - 1
	input1->inputSprite = CCSprite::create("Images/PhysicsTech/QualityInput.png");
	input1->inputSprite->setAnchorPoint(cocos2d::CCPointZero);
	input1->inputSprite->setPosition(ccp(input1->sprite->getPositionX() + 32 + 5, 95));
	input1->addChild(input1->inputSprite);
	//input - 1
	input1->text->setPosition(ccp(input1->sprite->getPositionX() + 32 + 25, 105));
	input1->inputRect = CCSize(100, 20);
	input1->text->setContentSize(input1->inputRect);		

	input1->OnEnter();

	this->addChild(input1);
}

void DBActionSprite::load()
{
    this->m_data->load(getTag());
    
}

DBActionSprite* DBActionSprite::create()
{
    DBActionSprite* pActionSprite = new DBActionSprite();
    if (pActionSprite)
    {
        pActionSprite->autorelease();
        pActionSprite->m_data = DBData::create();
        pActionSprite->m_data->retain();
        return pActionSprite;
    }
    CC_SAFE_DELETE(pActionSprite);
    return NULL;
}