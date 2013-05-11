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

	CCHttpRequest* request = new CCHttpRequest();

	std::vector<std::string> headers;

	request->setRequestType(op);

	std::string fullUrl = "https://api.parse.com" + url;
	request->setUrl(fullUrl.c_str());

	request->setResponseCallback(receiver, selector);

	CCHttpClient* client = CCHttpClient::getInstance();

	std::vector<char> requestData;
	switch(op)
	{
		case CCHttpRequest::kHttpGet:
			if (value.IsNull() == false)
			{
				ParseJson::ToByteArray(value, requestData, error);

				char* condition = curl_escape(&requestData.front(), requestData.size());
				request->setUrl((fullUrl + "?" + condition).c_str());
			}
			break;
		case CCHttpRequest::kHttpPost:

			headers.push_back("Content-Type: application/json");

			ParseJson::ToByteArray(value, requestData, error);
			request->setRequestData(&requestData.front(), requestData.size());
			break;
		case CCHttpRequest::kHttpPut:

			headers.push_back("Content-Type: application/json");

			ParseJson::ToByteArray(value, requestData, error);
			request->setRequestData(&requestData.front(), requestData.size());
			break;
		case CCHttpRequest::kHttpDelete:
			break;
	}

	headers.push_back(std::string("X-Parse-Application-Id: ") + this->_applicationId);
	headers.push_back(std::string("X-Parse-REST-API-Key: ") + this->_apiKey);
	request->setHeaders(headers);

	client->send(request);

	request->release();

	return error;
}
NS_CC_EXT_END
