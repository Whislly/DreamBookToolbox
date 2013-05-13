#ifndef __PARSE_ERROR__
#define __PARSE_ERROR__

#include "cocos2d.h"
#include "ExtensionMacros.h"

NS_CC_EXT_BEGIN

class ParseError
{
public:
	ParseError(void);
	virtual ~ParseError(void);

	void SetError(const char* str);

	void SetError(const std::string& str);

	const std::string& GetError();

	std::string error;
};

NS_CC_EXT_END

#endif
