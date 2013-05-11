#ifndef __CustomUserData_Common_H__
#define __CustomUserData_Common_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"

#include "Define.h"

USING_NS_CC;

class CustomUserData
{
public:
		int id;
		int groupId;
		bool draw;
		ccColor4F color;

		CustomUserData(void);
		~CustomUserData(void);
};

#endif
