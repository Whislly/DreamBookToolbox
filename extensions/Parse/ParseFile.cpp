#include "ParseFile.h"
#include "ParseJson.h"
#include "ParseManager.h"
#include <string.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define strcasecmp stricmp
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

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

	const char* error = 0;
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
		fseek(file, 0, SEEK_SET);
		buffer = new char[size];
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
		fclose(file);
		file = 0;
		ParseManager::instance()->request(CCHttpRequest::kHttpPost,
			std::string("/1/files/") + fileName,
			buffer,
			size,
			this,
			(SEL_CallFuncND)&ParseFile::uploadFileFinished,
			getContentType(fileName));
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
		(SEL_CallFuncND)&ParseFile::uploadFileContentFinished,
		getContentType(fileName));
}

void ParseFile::deleteFile(const char* fileName)
{
	ParseManager::instance()->request(CCHttpRequest::kHttpDelete,
		std::string("/1/files/") + fileName,
		0,
		0,
		this,
		(SEL_CallFuncND)&ParseFile::deleteFileFinished,
		0,
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
		(SEL_CallFuncND)&ParseFile::downloadFileFinished,
		0,
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

const char* ParseFile::getContentType(const char* fileName)
{
	const char* contentType = "Content-Type: text/plain";
	const char* ext = strrchr(fileName, '.');
	if (ext)
	{
		if(strcasecmp(ext, ".txt") == 0)
		{
			contentType = "Content-Type: text/plain";
		}
		else if (strcasecmp(ext, ".jpg") == 0)
		{
			contentType = "Content-Type: image/jpeg";
		}
		else if (strcasecmp(ext, ".png") == 0)
		{
			contentType = "Content-Type: image/png";
		}
		else if (strcasecmp(ext, ".xml") == 0)
		{
			contentType = "Content-Type: application/xml";
		}
		else if (strcasecmp(ext, ".zip") == 0)
		{
			contentType = "Content-Type: application/x-zip";
		}
		else if (strcasecmp(ext, ".json") == 0)
		{
			contentType = "Content-Type: application/json";
		}
	}
	return contentType;
}

NS_CC_EXT_END
