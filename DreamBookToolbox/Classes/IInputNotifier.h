#ifndef __IInputNotifier_INF_H__
#define __IInputNotifier_INF_H__

#include "cocos2d.h"
#include "CommonHelper.h"
#include "Define.h"

USING_NS_CC;

class IInputNotifier
{
public:
		virtual void ClickOnSprite(CCPoint location, CCSprite *sprite) = 0;
};

#endif
