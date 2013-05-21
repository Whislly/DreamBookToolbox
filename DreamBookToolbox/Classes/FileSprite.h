#ifndef __FileSprite_Sprite_H_
#define __FileSprite_Sprite_H_

#include "cocos2d.h"

USING_NS_CC;

class FileSprite : public CCSprite
{
public:
	char path[255];

	FileSprite(void);
	~FileSprite(void);

	CREATE_FUNC(FileSprite);
};

#endif
