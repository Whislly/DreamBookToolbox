#ifndef __PARSE_OBJECT__
#define __PARSE_OBJECT__

#include "cocos2d.h"
#include "ExtensionMacros.h"
#include "document.h"
#include "ParseError.h"
#include "Delegate.h"

NS_CC_EXT_BEGIN

class ParseQuery;

class ParseObject : public CCObject
{
public:
	typedef rapidjson::Value ValueType;
	typedef rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> AllocatorType;

public:
	ParseObject(const char* className);
	virtual ~ParseObject(void);

	const std::string& objectId() const { return this->_objectId; }
	const std::string& createdAt() const { return this->_createdAt; }
	const std::string& updatedAt() const { return this->_updatedAt; }

	//ValueType& operator[](const char* key)
	//{
	//	ValueType& data = this->_value[key];
	//	if (data.IsNull())
	//	{
	//		this->_value.AddMember(key, data, this->_allocator);
	//		return this->_value[key];
	//	}
	//	return data;
	//}
	template <typename T>
	void Set(const char* key, T value)
	{
		this->_value[key] = value;
	}

	template <typename T>
	T Get(const char* key)
	{
		return (T)this->_value[key];
	}

	template <typename T>
	ParseObject& Add(const char* key, T value) 
	{
		this->_value.AddMember(key, value, this->_allocator);
		return *this;
	}

	void save();
	Delegate<bool, ParseError*> saveCompleted;

	void erase();
	Delegate<bool, ParseError*> eraseCompleted;

	void toString(std::string& str);
	
private:
	void set(const char* key, ValueType value);
	void set(const char* key, ValueType& value);
	ParseObject& add(const char* key, ValueType value);
	ParseObject& add(const char* key, ValueType& value);
	ValueType Get(const char* key);

private:
	ParseObject(const char* className, ValueType& value);
	void createObject();
	void updateObject();

	void createObjectFinished(CCNode* sender, void* param);
	void updateObjectFinished(CCNode* sender, void* param);
	void eraseFinished(CCNode* sender, void* param);

private:
	bool isBusy;

	std::string _className;
	std::string _objectId;
	std::string _createdAt;
	std::string _updatedAt;

	ValueType _value;
	AllocatorType _allocator;

	friend class ParseQuery;
};

NS_CC_EXT_END

#endif