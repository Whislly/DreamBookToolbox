#include "DesignLayer.h"
#include "DBActionSprite.h"

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
{
}

DesignLayer::~DesignLayer()
{
    if(m_lblTime)
    {
        m_lblTime->release();
    }
    m_lblTime = NULL;
}

void DesignLayer::addSpriteEx( cocos2d::CCSprite* pSprite )
{
    this->addChild(pSprite, 0, pSprite->getTag());
}

void DesignLayer::addSpriteEx( cocos2d::CCSprite* pSprite, int zOrder )
{
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
        (CCActionInterval*)CCSequence::create(CCScaleTo::create(0.3f, scaleValue + 0.1f), 
         CCScaleTo::create(0.3f, scaleValue - 0.1f), NULL)
        );
    pSprite->runAction(pAction);
}

void DesignLayer::designSpriteStatus( int tag )
{
    setEnableTime(false);
    m_currentSpriteTag = tag;

    this->stopChildrenActions(tag);
    DBActionSprite* pActionSprite = (DBActionSprite*)(this->getChildByTag(tag));
    pActionSprite->designStatus(m_time);
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

void DesignLayer::activeActions( int tag )
{
    m_currentSpriteTag = tag;
    m_lblTime->setString("Time: 0");
    m_time = 0.0f;

    DBActionSprite* pActionSprite = (DBActionSprite*)(this->getChildByTag(tag));
    if (pActionSprite)
    {
        /*m_time = pActionSprite->getStartTime();
        char time_str[20];
        sprintf(time_str, "Time: %.1f", this->m_time);
        m_lblTime->setString(time_str);*/
        pActionSprite->clearAction(m_time);
        pActionSprite->designAction(this->m_time);
        this->focusChild(pActionSprite);
    }
    this->runChildrenActions(tag);
    setEnableTime(true);
}

void DesignLayer::step( float time )
{
    int temp = (int)(time * 10);
    this->m_time += (temp * 0.1);

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
