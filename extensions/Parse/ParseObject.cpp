#include "ParseObject.h"
#include "ParseJson.h"
#include "ParseManager.h"

NS_CC_EXT_BEGIN

ParseObject::ParseObject(const char* className)
	: _objectId("")
	, _createdAt("")
	, _updatedAt("")
{
	this->_className = className;
	this->_value.SetObject();
}

ParseObject::ParseObject(const char* className, ParseObject::ValueType& value)
	: _objectId("")
	, _createdAt("")
	, _updatedAt("")
{
	this->_className = className;
	
	this->_objectId = value["objectId"].GetString();
	this->_updatedAt = value["updatedAt"].GetString();
	this->_createdAt = value["createdAt"].GetString();
	value.RemoveMember("objectId");
	value.RemoveMember("updatedAt");
	value.RemoveMember("createdAt");

	new(&this->_value)ParseObject::ValueType(value, this->_allocator);
}


ParseObject::~ParseObject(void)
{
}

void ParseObject::save()
{
	if (this->_objectId.empty())
	{
		this->createObject();
	}
	else
	{
		this->updateObject();
	}
}

void ParseObject::createObject()
{
	ParseManager::instance()->request(CCHttpRequest::kHttpPost,
			"/1/classes/" + this->_className,
			this->_value,
			this,
			(SEL_CallFuncND)&ParseObject::createObjectFinished);
}

void ParseObject::updateObject()
{
	ParseManager::instance()->request(CCHttpRequest::kHttpPut,
			"/1/classes/" + this->_className + "/" + this->_objectId,
			this->_value,
			this,
			(SEL_CallFuncND)&ParseObject::updateObjectFinished);
}

void ParseObject::createObjectFinished(CCNode* sender, void* param)
{
	bool sucess = false;
	ParseError* error = new ParseError();

	CCHttpResponse* response = (CCHttpResponse*)param;
	
	if (response->getResponseCode() == 201)
	{
		rapidjson::Document retValue;
		if(ParseJson::FromByteArray(*response->getResponseData(), retValue, error))
		{
			if (retValue.HasMember("createdAt"))
			{
				this->_createdAt = retValue["createdAt"].GetString();
				this->_objectId = retValue["objectId"].GetString();
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

	this->saveCompleted(sucess, error);

	delete error;
}

void ParseObject::updateObjectFinished(CCNode* sender, void* param)
{
	bool sucess = false;
	ParseError* error = new ParseError();

	CCHttpResponse* response = (CCHttpResponse*)param;
	if (response->getResponseCode() == 200)
	{
		rapidjson::Document retValue;
		if (ParseJson::FromByteArray(*response->getResponseData(), retValue, error))
		{
			if (retValue.HasMember("updatedAt"))
			{
				this->_updatedAt = retValue["updatedAt"].GetString();
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

	this->saveCompleted(sucess, error);

	delete error;
}

void ParseObject::erase()
{
	ParseManager::instance()->request(CCHttpRequest::kHttpDelete,
		"/1/classes/" + this->_className + "/" + this->_objectId,
		this->_value,
		this,
		(SEL_CallFuncND)&ParseObject::eraseFinished);
}

void ParseObject::eraseFinished(CCNode* sender, void* param)
{
	bool sucess = false;
	ParseError* error = new ParseError();

	CCHttpResponse* response = (CCHttpResponse*)param;
	if (response->getResponseCode() == 200)
	{
		rapidjson::Document retValue;
		if (ParseJson::FromByteArray(*response->getResponseData(), retValue, error))
		{
			if (retValue.IsNull())
			{
				this->_objectId = "";
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

	this->eraseCompleted(sucess, error);
		
	delete error;
}

void ParseObject::toString(std::string& str)
{
	std::vector<char> byteArray;
	ParseJson::ToByteArray(this->_value, byteArray, 0);
	byteArray.push_back('\0');
	str = &byteArray.front();
}

NS_CC_EXT_END