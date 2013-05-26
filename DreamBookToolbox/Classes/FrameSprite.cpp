#include "FrameSprite.h"
#include "Define.h"

FrameSprite::FrameSprite(void)
{
	this->pathArray = NULL;
	this->pathArray = CCArray::create();
	this->pathArray->retain();
}

FrameSprite::~FrameSprite()
{
    CCARRAY_RELEASE(pathArray);
}

void FrameSprite::AddPath(char* path)
{
	CCString *pathObj = CCString::create(path);
	pathObj->retain();
    pathArray->addObject(pathObj);
}

const char* FrameSprite::objectInfor()
{
    return "DBT.FrameSprite";
}
