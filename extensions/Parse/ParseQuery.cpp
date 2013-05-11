#include "ParseQuery.h"
#include "ParseJson.h"
#include "ParseObject.h"
#include "ParseManager.h"

NS_CC_EXT_BEGIN

ParseQuery::ParseQuery(const char* className)
{
	this->_className = className;
}

ParseQuery::~ParseQuery(void)
{
}


void ParseQuery::findObjects()
{
	ParseManager::instance()->request(CCHttpRequest::kHttpGet,
		"/1/classes/" + this->_className,
		this->_value,
		this,
		(SEL_CallFuncND)&ParseQuery::findObjectsFinished);
}

void ParseQuery::findObjectWithId(const char* objectId)
{
	ParseManager::instance()->request(CCHttpRequest::kHttpGet,
		"/1/classes/" + this->_className + "/" + objectId,
		this->_value,
		this,
		(SEL_CallFuncND)&ParseQuery::findObjectWithIdFinished);
}

void ParseQuery::findObjectsFinished(CCNode* sender, void* param)
{
	CCHttpResponse* response = (CCHttpResponse*)param;

	CCArray* array = 0;
	
	if (response->getResponseCode() == 200)
	{
		rapidjson::Document retValue;
		if (ParseJson::FromByteArray(*response->getResponseData(), retValue, NULL))
		{
			array = CCArray::create();
			array->init();

			rapidjson::Value& resultArray = retValue["results"];
			for (unsigned int i = 0; i < resultArray.Size(); ++i)
			{
				ParseObject* obj = new ParseObject(this->_className.c_str(), resultArray[i]);
				array->addObject(obj);
				obj->release();
			}
		}
	}

	this->findObjectsCompleted(this, array);
}

void ParseQuery::findObjectWithIdFinished(CCNode* sender, void* param)
{
	ParseObject* obj = 0;
	CCHttpResponse* response = (CCHttpResponse*)param;
	if (response->getResponseCode() == 200)
	{
		rapidjson::Document retValue;
		if (ParseJson::FromByteArray(*response->getResponseData(), retValue, NULL))
		{
			if (retValue.IsNull() == false)
			{
				obj = new ParseObject(this->_className.c_str(), retValue);
			}
		}
	}
	this->findObjectWithIdCompleted(this, obj);
}


NS_CC_EXT_END