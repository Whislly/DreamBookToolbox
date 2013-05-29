#include "DBPropertyData.h"

USING_NS_CC;

DBPropertyData::DBPropertyData()
    : m_pos(ccp(-999, -999))
    , m_scale(1.0f)
    , m_inputContent(NULL)
{

}

DBPropertyData::~DBPropertyData()
{
    if (m_inputContent)
    {
        m_inputContent->release();
        m_inputContent = NULL;
    }
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

    if(m_inputContent)
    {
        sprintf(key, "tag%d_time%d_input", tag, time);
        pUserData->setStringForKey(key, m_inputContent->getCString());
    }
}

void DBPropertyData::load( int tag, int time )
{
    CCUserDefault* pUserData = CCUserDefault::sharedUserDefault();
    char key[255];
    char content[255];

    sprintf(key, "tag%d_time%d_position", tag, time);
    sprintf(content, "%s", pUserData->getStringForKey(key).c_str());

    int idx = 0;
    int count = strlen(content);
    for (; idx < count; idx++)
    {
        if (content[idx] == ',')
        {
            break;
        }
    }
    m_pos.x = CCString::createWithData((const unsigned char*)(content + 1), idx - 1)->floatValue();
    m_pos.y = CCString::createWithData((const unsigned char*)(content + idx + 1), count - idx - 2)->floatValue();

    sprintf(key, "tag%d_time%d_scale", tag, time);
    m_scale = pUserData->getFloatForKey(key);

    sprintf(key, "tag%d_time%d_input", tag, time);
    m_inputContent = CCString::create(pUserData->getStringForKey(key));
    m_inputContent->retain();
}

void DBPropertyData::setInputContent( const char* content )
{
    if (m_inputContent)
    {
        m_inputContent->release();
    }
    m_inputContent = CCString::create(content);
    m_inputContent->retain();
}
