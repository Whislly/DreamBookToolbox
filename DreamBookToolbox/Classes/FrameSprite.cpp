#include "FrameSprite.h"

FrameSprite::FrameSprite(void)
{
	this->pathArray = NULL;
	this->pathArray = CCArray::create();
	this->pathArray->retain();
}

FrameSprite::~FrameSprite()
{
	if (this->pathArray != NULL)
	{
		for (int i = 0; i < this->pathArray->count(); i++)
		{
			CCString* pathObj = (CCString*)this->pathArray->objectAtIndex(i);
			pathObj->release();
		}
		this->pathArray->release();
	}
}

void FrameSprite::AddPath(char* path)
{
	CCString *pathObj = CCString::create(path);

	pathObj->retain();
}

const char* FrameSprite::objectInfor()
{
    return "DBT.FrameSprite";
}
