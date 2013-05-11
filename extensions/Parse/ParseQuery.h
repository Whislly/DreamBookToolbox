#ifndef __PARSE_QUERY__
#define __PARSE_QUERY__

#include "cocos2d.h"
#include "ExtensionMacros.h"
#include "rapidjson\document.h"
#include "ParseError.h"
#include "Delegate.h"
#include "cocoa\CCArray.h"

NS_CC_EXT_BEGIN

class ParseObject;

class ParseQuery : public CCObject
{
public:
	typedef rapidjson::Value ValueType;
	typedef rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> AllocatorType;

public:
	ParseQuery(const char* className);
	virtual ~ParseQuery(void);

	void findObjects();
	Delegate<CCArray*> findObjectsCompleted;

	void findObjectWithId(const char* objectId);
	Delegate<ParseObject*> findObjectWithIdCompleted;

private:
	void findObjectsFinished(CCNode* sender, void* param);
	void findObjectWithIdFinished(CCNode* sender, void* param);

private:
	std::string _className;
	ValueType _value;
};

NS_CC_EXT_END

#endif

