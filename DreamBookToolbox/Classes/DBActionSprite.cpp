#include "DBActionSprite.h"

USING_NS_CC;

DBActionSprite::DBActionSprite()
    : m_time(0.0f)
    , m_status(kDesignStatus)
    , m_data(NULL)
    , m_startTime(0.0f)
    , m_endTime(0.0f)
{

}

DBActionSprite::~DBActionSprite()
{
    if(m_data)
    {
        m_data->release();
    }
    m_data = NULL;
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
        pActionSprite->m_data = DBData::create();
        pActionSprite->m_data->retain();
        return pActionSprite;
    }
    CC_SAFE_DELETE(pActionSprite);
    return NULL;
}

bool DBActionSprite::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    this->stopAllActions();
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

void DBActionSprite::transform( float time )
{
    DBPropertyData* propertyData = m_data->getDBPropertyData(time);
    CCPoint pos = propertyData->getPosition();
    if ((pos.x != -999) && (pos.y != -999))
    {
        this->runAction(CCMoveTo::create(0.1f, pos));
    }
}

void DBActionSprite::step( float time )
{
    int temp = (int)(time * 10);
    this->m_time += (temp * 0.1);

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