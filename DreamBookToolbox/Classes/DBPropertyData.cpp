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

void DBPropertyData::save( rapidjson::PrettyWriter<rapidjson::FileStream>& write )
{
	write.String("X");
	write.Double(m_pos.x);

	write.String("Y");
	write.Double(m_pos.y);
   
	write.String("Scale");
	write.Double(m_scale);

	write.String("Input");
    write.String(m_inputContent?m_inputContent->getCString():"");
}

void DBPropertyData::load( rapidjson::Value& data )
{
    m_pos.x = data["X"].GetDouble();
    m_pos.y = data["Y"].GetDouble();

    m_scale = data["Scale"].GetDouble();

	m_inputContent = CCString::create(data["Input"].GetString());
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

const char* DBPropertyData::getInputContent()
{
    if (m_inputContent)
    {
        return m_inputContent->getCString();
    }
    else
    {
        return "";
    }
}
