#ifndef __PARSE_MANAGER__
#define __PARSE_MANAGER__

#include "cocos2d.h"
#include "ExtensionMacros.h"
#include "ParseError.h"
#include "network/HttpClient.h"
#include "rapidjson\document.h"

NS_CC_EXT_BEGIN

class ParseManager : public CCObject
{
public:
	ParseManager(void);
	~ParseManager(void);

    /** Return the shared instance **/
    static ParseManager *instance();

	/// configuration
	std::string applicationId() const;
	void setApplicationId(const std::string &applicationId);
	std::string apiKey() const;
	void setApiKey(const std::string &apiKey);
	std::string masterKey() const;
	void setMasterKey(const std::string &masterKey);
	bool trace() const;
	void setTrace(bool trace);

	/// communication
	ParseError *request(CCHttpRequest::HttpRequestType op,
						const std::string &url,
						const rapidjson::Value& variant,
						CCObject *receiver,
						SEL_CallFuncND selector);

	/// communication
	ParseError *request(CCHttpRequest::HttpRequestType op,
						const std::string &url, 
						const char* buffer, 
						size_t size,
						CCObject *receiver,
						SEL_CallFuncND selector,
						const char* contentType = 0,
						bool setMasterKey = false,
						void* param = 0);

private:
	std::string _applicationId;
	std::string _apiKey;
	std::string _masterKey;
	bool _trace;
};

NS_CC_EXT_END

#endif

