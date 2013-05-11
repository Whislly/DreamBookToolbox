#include "ParseJson.h"
#include "rapidjson\writer.h"

NS_CC_EXT_BEGIN

class ByteArrayStream
{
public:
	ByteArrayStream(std::vector<char>& stream) :_stream(stream){}

	void Put(char c) { _stream.push_back(c); }

private:
	std::vector<char>& _stream;
};

ParseJson::ParseJson(void)
{
}


ParseJson::~ParseJson(void)
{
}


void ParseJson::ToByteArray(const rapidjson::Value& value, std::vector<char>& byteArray, ParseError* error)
{
	ByteArrayStream stream(byteArray);
	rapidjson::Writer<ByteArrayStream> write(stream);
	value.Accept(write);
}


bool ParseJson::FromByteArray(std::vector<char>& byteArray, rapidjson::Document& value, ParseError* error)
{
	bool sucess = true;

	byteArray.push_back('\0');
	value.ParseInsitu<0>(&(byteArray.front()));
	if (value.HasParseError())
	{
		sucess = false;
	}
	return sucess;
}

NS_CC_EXT_END