#include "CCPlaceInterval.h"

NS_CC_BEGIN
void cocos2d::CCPlaceInterval::update( float time )
{
    CC_UNUSED_PARAM(time);
    if (m_idx <= m_pointArray->count())
    {
        m_pTarget->setPosition(m_pointArray->getControlPointAtIndex(m_idx));
        m_idx++;
    }
}

CCObject* cocos2d::CCPlaceInterval::copyWithZone( CCZone *pZone )
{
    CCZone *pNewZone = NULL;
    CCPlaceInterval *pRet = NULL;

    if (pZone && pZone->m_pCopyObject) {
        pRet = (CCPlaceInterval*) (pZone->m_pCopyObject);
    } else {
        pRet = new CCPlaceInterval();
        pZone = pNewZone = new CCZone(pRet);
    }

    CCActionInterval::copyWithZone(pZone);
    pRet->m_pointArray = m_pointArray;
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

CCPlaceInterval* cocos2d::CCPlaceInterval::create(float duration,  CCPointArray* pointArray )
{
    CCPlaceInterval *pRet = new CCPlaceInterval();

    if (pRet && (pRet->initWithDuration(duration))) 
    {
        pRet->m_pointArray = pointArray;
        pointArray->retain();
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return NULL;
}

CCPlaceInterval::CCPlaceInterval()
    : m_pointArray(NULL)
    , m_idx(0)
{

}

CCPlaceInterval::~CCPlaceInterval()
{
    if (m_pointArray)
    {
        m_pointArray->release();
        m_pointArray = NULL;
    }
}

NS_CC_END