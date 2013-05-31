#include "DBData.h"
#include "CommonHelper.h"

USING_NS_CC;

DBData::DBData()
    : m_dic(NULL)
    , m_startTime(0.0f)
    , m_endTime(0.0f)
    , m_resourceFileArray(NULL)
{
    m_resourceFileArray = CCArray::createWithCapacity(1);
    m_resourceFileArray->retain();
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

    CCARRAY_RELEASE(m_resourceFileArray);
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

void DBData::save( rapidjson::PrettyWriter<rapidjson::FileStream>& write )
{
	write.String("File");
	write.StartArray();
    if(m_resourceFileArray)
    {
		CCObject* pObj;
        CCARRAY_FOREACH(m_resourceFileArray, pObj)
        {
            CCString* str = (CCString*)pObj;
			write.String(str->getCString(), (size_t)str->length());
        }
    }
	write.EndArray();

	write.String("Data");
    CCDictElement* pElement;
	write.StartArray();
    CCDICT_FOREACH(m_dic, pElement)
    {
		write.StartObject();

		write.String("Time");
		write.Int(pElement->getIntKey());

        DBPropertyData* pPropertyData = (DBPropertyData*)pElement->getObject();
        pPropertyData->save(write);
		
		write.EndObject();
    }
    write.EndArray();
}

void DBData::load( rapidjson::Value& value )
{
	rapidjson::Value& fileArray = value["File"];
	if (fileArray.IsArray())
	{
		for (unsigned int i = 0; i < fileArray.Size(); ++i)
		{
			CCString* pStr = CCString::create(fileArray[i].GetString());
            m_resourceFileArray->addObject(pStr);
		}
	}

	if (m_dic)
    {
        m_dic->removeAllObjects();
    }
    else
    {
        m_dic = CCDictionary::create();
    }

	rapidjson::Value& dataArray = value["Data"];
	if (dataArray.IsArray())
	{
		for (unsigned int i = 0; i < dataArray.Size(); ++i)
		{
			rapidjson::Value& data = dataArray[i];
			DBPropertyData* pPropertyData = DBPropertyData::create();
			pPropertyData->load(data);
			m_dic->setObject(pPropertyData, data["Time"].GetInt());
		}

	}
}

void DBData::addResourcePath( char* path )
{
    m_resourceFileArray->addObject(CCString::create(path));
}

void DBData::addResourcePath( CCArray* pathArray )
{
    m_resourceFileArray->addObjectsFromArray(pathArray);
}
