#ifndef __FrameSprite_Sprite_H_
#define __FrameSprite_Sprite_H_

#include "cocos2d.h"

USING_NS_CC;

class PathObject : public CCObject
{
public:
	char path[255];
};

class FrameSprite : public CCSprite
{
public:
	CCArray *pathArray;

	FrameSprite(void);
	~FrameSprite(void);

	CREATE_FUNC(FrameSprite);
	
	virtual const char* description(void);

	void AddPath(char* path);
};

#endif
