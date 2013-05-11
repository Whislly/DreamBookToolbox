#ifndef __PARSE__
#define __PARSE__

#include "cocos2d.h"
#include "ExtensionMacros.h"

NS_CC_EXT_BEGIN

class Parse : public CCObject
{
public:
	Parse(void);
	~Parse(void);

	/// configuration
	std::string applicationId() const;
	void setApplicationId(const std::string &applicationId);
	std::string apiKey() const;
	void setApiKey(const std::string &apiKey);
	bool trace() const;
	void setTrace(bool trace);
};

NS_CC_EXT_END

#endif
