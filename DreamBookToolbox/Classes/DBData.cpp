#include "DBData.h"

USING_NS_CC;

DBData::DBData()
    : m_dic(NULL)
    , m_startTime(0.0f)
    , m_endTime(0.0f)
    , m_resourceFileArray(NULL)
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
    float tempTime = temp * 0.1f;
    if (m_startTime > tempTime)
    {
        m_startTime = tempTime;
    }
    if (m_endTime < tempTime)
    {
        m_endTime = tempTime;
    }
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

void DBData::save(int tag)
{
    CCUserDefault* pUserData = CCUserDefault::sharedUserDefault();
    CCObject* pObj = NULL;
    char key[255] = {0};
    int idx = 0;
    if(m_resourceFileArray)
    {
        sprintf(key, "tag%d_frameCount", tag);
        pUserData->setIntegerForKey(key, m_resourceFileArray->count());
        CCARRAY_FOREACH(m_resourceFileArray, pObj)
        {
            CCString* str = (CCString*)pObj;
            sprintf(key, "tag%d_frame%d", tag, idx);
            pUserData->setStringForKey(key, str->m_sString);
            idx++;
        }
    }

    char content[255] = {0};
    idx = 0;
    CCArray* keyArrays = m_dic->allKeys();
    CCARRAY_FOREACH(keyArrays, pObj)
    {
        CCInteger* pKey = (CCInteger*)pObj;
        sprintf(key, "tag%d_propertyDataArray%d", tag, idx);
        sprintf(content, "%s,%d", content, pKey->getValue());
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

    CCDictElement* pElement;
    CCDICT_FOREACH(m_dic, pElement)
    {
        DBPropertyData* pPropertyData = (DBPropertyData*)pElement->getObject();
        pPropertyData->save(tag, pElement->getIntKey());
    }
    pUserData->flush();
}

void DBData::load( int tag )
{

}
