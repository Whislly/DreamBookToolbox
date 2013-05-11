#include "DBData.h"

USING_NS_CC;

DBData::DBData()
    : m_dic(NULL)
{

}

DBData::~DBData()
{
    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(m_dic, pElement)
    {
        CCObject* pObj = pElement->getObject();
        if (pObj)
        {
            pObj->release();
        }
    }
    if (m_dic)
    {
        m_dic->release();
    }
    m_dic = NULL;
}

bool DBData::init()
{
    m_dic = new CCDictionary();
    m_dic->retain();
    DBPropertyData* pData = DBPropertyData::create();
    this->addDBPropertyData(0.0f, pData);
    return true;
}

DBData* DBData::create()
{
    DBData* pData = new DBData();
    if(pData && pData->init())
    {
        pData->autorelease();
        return pData;
    }
    CC_SAFE_DELETE(pData);
    return NULL;
}

void DBData::addDBPropertyData( float time, DBPropertyData* propertyData )
{
    int temp = (int)(time * 10.0f);
    this->m_dic->setObject(propertyData, time);
    propertyData->retain();
}

DBPropertyData* DBData::getDBPropertyData( float time )
{
    int temp = (int)(time * 10.0f);
    DBPropertyData* pData = (DBPropertyData*)this->m_dic->objectForKey(temp);
    if (!pData)
    {
        pData = DBPropertyData::create();
        CCLOG("DBPropertyData, time: %f, key: %d", time, temp);
        this->m_dic->setObject(pData, temp);
        pData->retain();
    }
    return pData;
}

void DBData::removeDBPropertyData( float startTime, float endTime )
{
    int startIdx = (int)(startTime * 10.0f);
    int endIdx = (int)(endTime * 10.0f);
    for (int i = startIdx; i < endIdx + 1; i++)
    {
        CCObject* pObj = this->m_dic->objectForKey(i);
        if (pObj)
        {
            pObj->release();
        }
        this->m_dic->removeObjectForKey(i);
    }
}
