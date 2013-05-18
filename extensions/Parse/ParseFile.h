#ifndef __PARSE_FILE__
#define __PARSE_FILE__

#include "cocos2d.h"
#include "ExtensionMacros.h"
#include "rapidjson\document.h"
#include "Delegate.h"
#include "ParseError.h"

NS_CC_EXT_BEGIN

class FileInfo
{
public:
	FileInfo(const char* url, const char* fileName)
	{
		this->url = url;
		this->fileName = fileName;
	}

	const std::string& GetUrl() { return this->url; }
	const std::string& GetFileName() { return this->fileName; }

private:
	std::string url;
	std::string fileName;
};

class ParseFile : public CCObject
{
public:
	ParseFile(void);
	~ParseFile(void);

	void uploadFile(const char* dir, const char* fileName);
	Delegate<FileInfo*, ParseError*> uploadFileCompleted;

	void uploadFileContent(const char* fileName, char* content, size_t length);
	Delegate<FileInfo*, ParseError*> uploadFileContentCompleted;

	void downloadFile(const char* url, const char* savePathName);
	Delegate<const char*, ParseError*> downloadFileCompleted;

	void deleteFile(const char* fileName);
	Delegate<bool, ParseError*> deleteFileCompleted;
private:
	void uploadFileFinished(CCNode* sender, void* param);
	void uploadFileContentFinished(CCNode* sender, void* param);
	void downloadFileFinished(CCNode* sender, void* param);
	void deleteFileFinished(CCNode* sender, void* param);
	const char* getContentType(const char* fileName);
};

NS_CC_EXT_END

#endif