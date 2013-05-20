#ifndef __PARSE_JSON__
#define __PARSE_JSON__

#include "cocos2d.h"
#include "ExtensionMacros.h"
#include "document.h"
#include "ParseError.h"

NS_CC_EXT_BEGIN

class ParseJson
{
public:
	ParseJson(void);
	~ParseJson(void);

	static void ToByteArray(const rapidjson::Value& value, std::vector<char>& byteArray, ParseError* error);
	static bool FromByteArray(std::vector<char>& byteArray, rapidjson::Document& value, ParseError* error);
};

NS_CC_EXT_END

#endif