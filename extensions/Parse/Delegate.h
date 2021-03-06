#ifndef __DELEGATE__
#define __DELEGATE__

#include "cocos2d.h"
#include "ExtensionMacros.h"

NS_CC_EXT_BEGIN

template<class ObjectType, class ArgsType>
struct Delegate
{
	typedef void (*StaticFun)(ObjectType obj, ArgsType args);
	typedef void (CCObject::*MemberFun)(ObjectType sender, ArgsType args);

	CCObject* observer;
	union
	{
		StaticFun staticFun;
		MemberFun memberFun;
	};

	Delegate()
	{
		this->observer = 0;
		this->memberFun = 0;
	}

	~Delegate()
	{
		if (this->observer != 0)
		{
			this->observer->release();
		}
		this->observer = 0;
		this->memberFun = 0;
	}

	void Set(StaticFun staticFun)
	{
		this->~Delegate();
		this->observer = 0;
		this->staticFun = staticFun;
	}

	void Set(CCObject* observer, MemberFun memberFun)
	{
		this->~Delegate();
		this->observer = observer;
		this->memberFun = memberFun;

		this->observer->retain();
	}

	void operator ()(ObjectType obj, ArgsType args)
	{
		if ((this->observer != 0) && (this->memberFun != 0))
		{
			(this->observer->*this->memberFun)(obj, args);
		}
		else if (this->staticFun != 0)
		{
			(this->staticFun)(obj, args);
		}
	}
};

NS_CC_EXT_END

#endif