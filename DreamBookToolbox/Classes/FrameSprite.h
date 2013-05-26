#ifndef __FrameSprite_Sprite_H_
#define __FrameSprite_Sprite_H_

#include "cocos2d.h"

USING_NS_CC;

class FrameSprite : public CCSprite
{
public:
	CCArray *pathArray;

	FrameSprite(void);
	~FrameSprite(void);

	CREATE_FUNC(FrameSprite);
	
	void AddPath(char* path);

    virtual const char* objectInfor();
};

#endif
