#include "ParseError.h"

NS_CC_EXT_BEGIN

ParseError::ParseError(void)
{
}


ParseError::~ParseError(void)
{
}

void ParseError::SetError(const char* str)
{
	this->error = str;
}

void ParseError::SetError(const std::string& str)
{
	this->error = str;
}

const std::string& ParseError::GetError()
{
	return this->error;
}

NS_CC_EXT_END
