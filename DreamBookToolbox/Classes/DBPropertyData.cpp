#include "DBPropertyData.h"

USING_NS_CC;

DBPropertyData::DBPropertyData()
    : m_pos(ccp(-999, -999))
    , m_scale(1.0f)
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

void DBPropertyData::save( int tag, int time )
{
    CCUserDefault* pUserData = CCUserDefault::sharedUserDefault();
    char key[255];
    char content[255];
    sprintf(key, "tag%d_time%d_position", tag, time);
    sprintf(content, "(%.3f,%.3f)", m_pos.x, m_pos.y);
    pUserData->setStringForKey(key, content);

    sprintf(key, "tag%d_time%d_scale", tag, time);
    pUserData->setFloatForKey(key, m_scale);
}

void DBPropertyData::load( int tag, int time )
{
    CCUserDefault* pUserData = CCUserDefault::sharedUserDefault();
    char key[255];
    char content[255];

    sprintf(key, "tag%d_time%d_position", tag, time);
    sprintf(content, "(%.3f,%.3f)", m_pos.x, m_pos.y);
    const char* pPosStr = pUserData->getStringForKey(key, content).c_str();

    int idx = 0;
    int count = strlen(pPosStr);
    for (; idx < count; idx++)
    {
        if (pPosStr[idx] == ',')
        {
            break;
        }
    }
    m_pos.x = CCString::createWithData((const unsigned char*)(pPosStr + 1), idx - 1)->floatValue();
    m_pos.y = CCString::createWithData((const unsigned char*)(pPosStr + idx + 1), count - idx - 2)->floatValue();

    sprintf(key, "tag%d_time%d_scale", tag, time);
    m_scale = pUserData->getFloatForKey(key);
}
