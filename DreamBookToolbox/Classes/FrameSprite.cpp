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
			PathObject *pathObj = (PathObject*)this->pathArray->objectAtIndex(i);
			pathObj->release();
		}
		this->pathArray->release();
	}
}

void FrameSprite::AddPath(char* path)
{
	PathObject *pathObj = new PathObject();
	pathObj->autorelease();
	sprintf(pathObj->path, path);

	pathObj->retain();
}

const char* FrameSprite::description(void)
{
	return "DBT.FrameSprite";
}
