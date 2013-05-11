#include "DBPropertyData.h"

USING_NS_CC;

DBPropertyData::DBPropertyData()
    : m_pos(ccp(-999, -999))
{

}

DBPropertyData::~DBPropertyData()
{

}

DBPropertyData* DBPropertyData::create()
{
    DBPropertyData* pData = new DBPropertyData();
    if(pData)
    {
        pData->autorelease();
        return pData;
    }
    CC_SAFE_DELETE(pData);
    return NULL;
}

cocos2d::CCPoint DBPropertyData::getPosition()
{
    return m_pos;
}

void DBPropertyData::setPosition( cocos2d::CCPoint& pos )
{
    m_pos = CCPoint(pos.x, pos.y);
}
