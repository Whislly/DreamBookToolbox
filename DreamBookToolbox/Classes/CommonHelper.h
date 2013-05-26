#ifndef __CommonHelper_Common_H__
#define __CommonHelper_Common_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"

#include "Define.h"

USING_NS_CC;

class CommonHelper
{
public:
		CommonHelper(void);

		static int GetRandomNum(int min, int max);

		static CCRect GetRect(CCNode *node);

		static b2World* CreateWorld();

		static void DrawNode(CCNode *node);

		static CCSize CountContainerNumber(CCSize containerSize, int width, int xInterval, int xMargin, int height, int yInterval, int yMargin);

		static CCSprite* CloneSprite(CCSprite *sprite);

        static CCArray* getIntArray(char* content);
};

#endif
