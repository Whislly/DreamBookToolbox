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
	CCArray* array = 0;
	ParseError* error = new ParseError();
	CCHttpResponse* response = (CCHttpResponse*)param;

	bool sucess = false;
	if (response->getResponseCode() == 200)
	{
		rapidjson::Document retValue;
		if (ParseJson::FromByteArray(*response->getResponseData(), retValue, error))
		{
			array = CCArray::create();
			array->init();

			if (retValue.HasMember("results") && retValue["results"].IsArray())
			{
				rapidjson::Value& resultArray = retValue["results"];
				for (unsigned int i = 0; i < resultArray.Size(); ++i)
				{
					ParseObject* obj = new ParseObject(this->_className.c_str(), resultArray[i]);
					array->addObject(obj);
					obj->release();
				}
				sucess = true;
			}
		}
		if (sucess == false)
		{
			std::vector<char>* responseData = response->getResponseData();
			responseData->push_back('\0');
			error->SetError(error->GetError() + "data:" + &responseData->front());
		}
	}
	else
	{
		if (strlen(response->getErrorBuffer()) > 0)
		{
			error->SetError(response->getErrorBuffer());
		}
		else
		{
			std::vector<char>* responseData = response->getResponseData();
			responseData->push_back('\0');
			error->SetError(&responseData->front());
		}
	}

	this->findObjectsCompleted(array, error);

	delete error;
}

void ParseQuery::findObjectWithIdFinished(CCNode* sender, void* param)
{
	ParseError* error = new ParseError();
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
	if (strlen(response->getErrorBuffer()) > 0)
	{
		error->SetError(response->getErrorBuffer());
	}
	else
	{
		std::vector<char>* responseData = response->getResponseData();
		responseData->push_back('\0');
		error->SetError(&responseData->front());
	}

	this->findObjectWithIdCompleted(obj, error);

	delete error;
}


NS_CC_EXT_END