#include "ParseFile.h"
#include "ParseJson.h"
#include "ParseManager.h"

NS_CC_EXT_BEGIN

ParseFile::ParseFile(void)
{
}


ParseFile::~ParseFile(void)
{
}

void ParseFile::uploadFile(const char* dir, const char* fileName)
{
	std::string pathName = dir;
	pathName += "/";
	pathName += fileName;

	char* error = 0;
	FILE* file = 0;
	char* buffer = 0;
	do
	{
		file = fopen(pathName.c_str(), "rb");
		if (file == 0)
		{
			error = "open file error!";
			break;
		}
		fseek(file, 0, SEEK_END);
		size_t size = ftell(file);
		buffer = new char(size);
		if (buffer == 0)
		{
			error = "malloc buffer error!";
			break;
		}
		if (fread(buffer, 1, size, file) != size)
		{
			error = "read file error!";
			break;
		}
		ParseManager::instance()->request(CCHttpRequest::kHttpPost,
			std::string("/1/files/") + fileName,
			buffer,
			size,
			this,
			(SEL_CallFuncND)&ParseFile::uploadFileFinished);
	}while(0);

	if (error)
	{
		FileInfo* result = 0;
		ParseError* parseError = new ParseError();
		parseError->SetError(error);

		this->uploadFileCompleted(result, parseError);

		delete parseError;
	}

	if (file)
	{
		fclose(file);
	}
	if (buffer)
	{
		delete buffer;
	}
}

void ParseFile::uploadFileContent(const char* fileName, char* content, size_t length)
{
	ParseManager::instance()->request(CCHttpRequest::kHttpPost,
		std::string("/1/files/") + fileName,
		content,
		length,
		this,
		(SEL_CallFuncND)&ParseFile::uploadFileContentFinished);
}

void ParseFile::deleteFile(const char* fileName)
{
	ParseManager::instance()->request(CCHttpRequest::kHttpDelete,
		std::string("/1/files/") + fileName,
		0,
		0,
		this,
		(SEL_CallFuncND)&ParseFile::deleteFileFinished,
		true);
}

void ParseFile::deleteFileFinished(CCNode* sender, void* param)
{
	
	bool success = false;
	ParseError* error = new ParseError();

	CCHttpResponse* response = (CCHttpResponse*)param;
	
	if (response->getResponseCode() == 200)
	{
		success = true;
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

	this->deleteFileCompleted(success, error);

	delete error;
}

void ParseFile::uploadFileFinished(CCNode* sender, void* param)
{
	FileInfo* result = 0;
	ParseError* error = new ParseError();

	CCHttpResponse* response = (CCHttpResponse*)param;
	
	if (response->getResponseCode() == 201)
	{
		rapidjson::Document retValue;
		if(ParseJson::FromByteArray(*response->getResponseData(), retValue, error))
		{
			if (retValue.HasMember("url"))
			{
				result = new FileInfo(retValue["url"].GetString(), retValue["name"].GetString());
			}
		}
		if (result == 0)
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

	this->uploadFileCompleted(result, error);

	delete error;
}

void ParseFile::uploadFileContentFinished(CCNode* sender, void* param)
{
	FileInfo* result = 0;
	ParseError* error = new ParseError();

	CCHttpResponse* response = (CCHttpResponse*)param;
	
	if (response->getResponseCode() == 201)
	{
		rapidjson::Document retValue;
		if(ParseJson::FromByteArray(*response->getResponseData(), retValue, error))
		{
			if (retValue.HasMember("url"))
			{
				result = new FileInfo(retValue["url"].GetString(), retValue["name"].GetString());
			}
		}
		if (result == 0)
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

	this->uploadFileContentCompleted(result, error);

	delete error;
}

void ParseFile::downloadFile(const char* url, const char* savePathName)
{
	ParseManager::instance()->request(CCHttpRequest::kHttpGet,
		url,
		0,
		0,
		this,
		(SEL_CallFuncND)&ParseFile::deleteFileFinished,
		false,
		(void*)savePathName);
}

void ParseFile::downloadFileFinished(CCNode* sender, void* param)
{
	const char* savePathName = 0;
	ParseError* error = new ParseError();

	CCHttpResponse* response = (CCHttpResponse*)param;
	
	if (response->getResponseCode() == 200)
	{
		FILE* file = 0;
		const char* errorMsg = 0;
		savePathName = (const char*)response->getHttpRequest()->getUserData();
		std::vector<char>* responseData = response->getResponseData();
		do
		{
			file = fopen(savePathName, "wb");
			if (file == 0)
			{
				errorMsg = "error open file!";
				break;
			}
			size_t writeLen = fwrite(&responseData->front(), 1, responseData->size(), file);
			if (writeLen != responseData->size())
			{
				errorMsg = "error write file!";
				break;
			}
		}while(0);

		if (file)
		{
			fclose(file);
		}
		if (errorMsg)
		{
			error->SetError(errorMsg);
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

	this->downloadFileCompleted(savePathName, error);

	delete error;
}

NS_CC_EXT_END
