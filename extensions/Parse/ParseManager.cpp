#include "ParseManager.h"
#include "ParseJson.h"
#include "curl/curl.h"

NS_CC_EXT_BEGIN

ParseManager::ParseManager(void)
{
}


ParseManager::~ParseManager(void)
{
}

 /** Return the shared instance **/
ParseManager *ParseManager::instance()
{
	static ParseManager theParseManager;

	return &theParseManager;
}

/// configuration
std::string ParseManager::applicationId() const
{
	return this->_applicationId;
}

void ParseManager::setApplicationId(const std::string &applicationId)
{
	this->_applicationId = applicationId;
}

std::string ParseManager::apiKey() const
{
	return this->_apiKey;
}

void ParseManager::setApiKey(const std::string &apiKey)
{
	this->_apiKey = apiKey;
}

std::string ParseManager::masterKey() const
{
	return this->_masterKey;
}

void ParseManager::setMasterKey(const std::string &masterKey)
{
	this->_masterKey = masterKey;
}

bool ParseManager::trace() const
{
	return this->_trace;
}

void ParseManager::setTrace(bool trace)
{
	this->_trace = trace;
}

/// communication
ParseError* ParseManager::request(CCHttpRequest::HttpRequestType op, 
								  const std::string &url, 
								  const rapidjson::Value& value,
								  CCObject *receiver, 
								  SEL_CallFuncND selector)
{
	ParseError* error = NULL;

	size_t size = 0;
	const char* buffer = 0;
	std::vector<char> requestData;
	if (value.IsNull() == false)
	{
		ParseJson::ToByteArray(value, requestData, error);
		buffer = &requestData.front();
		size = requestData.size();
	}
	
	this->request(op, url, buffer, size, receiver, selector);

	return error;
}

/// communication
ParseError* ParseManager::request(CCHttpRequest::HttpRequestType op, 
								  const std::string &url, 
								  const char* buffer,
								  size_t size,
								  CCObject *receiver, 
								  SEL_CallFuncND selector,
								  const char* contentType,
								  bool setMasterKey,
								  void* param)
{
	ParseError* error = NULL;

	CCHttpRequest* request = new CCHttpRequest();

	std::vector<std::string> headers;

	request->setRequestType(op);

	std::string fullUrl;
	if (strncmp(url.c_str(), "http", 4) != 0)
	{
		fullUrl = "https://api.parse.com" + url;
	}
	else
	{
		fullUrl = url;
	}

	request->setUrl(fullUrl.c_str());

	request->setResponseCallback(receiver, selector);

	request->setUserData(param);

	CCHttpClient* client = CCHttpClient::getInstance();


	if (contentType)
	{
		headers.push_back(contentType);
	}
	switch(op)
	{
		case CCHttpRequest::kHttpGet:
			if (size > 0)
			{
				char* condition = curl_escape(buffer, size);
				request->setUrl((fullUrl + "?" + condition).c_str());
			}
			break;
		case CCHttpRequest::kHttpPost:
			if (contentType == 0)
			{
				headers.push_back("Content-Type: application/json");
			}
			if (size > 0)
			{
				request->setRequestData(buffer, size);
			}
			break;
		case CCHttpRequest::kHttpPut:
			if (contentType == 0)
			{
				headers.push_back("Content-Type: application/json");
			}
			if (size > 0)
			{
				request->setRequestData(buffer, size);
			}
			break;
		case CCHttpRequest::kHttpDelete:
			break;
	}

	headers.push_back(std::string("X-Parse-Application-Id: ") + this->_applicationId);
	if (setMasterKey)
	{
		headers.push_back(std::string("X-Parse-Master-Key: ") + this->_masterKey);
	}
	else
	{
		headers.push_back(std::string("X-Parse-REST-API-Key: ") + this->_apiKey);
	}
	request->setHeaders(headers);

	client->send(request);

	request->release();

	return error;
}
NS_CC_EXT_END
