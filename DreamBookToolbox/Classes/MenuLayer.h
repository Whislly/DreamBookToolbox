#ifndef __MenuLayer_Layer_H__
#define __MenuLayer_Layer_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"
#include "CustomUserData.h"

#include "AppDelegate.h"

#include "Define.h"


#include "SimpleAudioEngine.h"

USING_NS_CC;

class MenuLayer :
		public CCLayer
{
private:
		b2World *world;
		CCSprite *startImage;
		CCSprite *pauseImage;
		CCMenuItemImage *startPauseItem;

		void Back(CCObject* pSender);
		void StartPause(CCObject* pSender);
		void Reset(CCObject* pSender);

public:
		virtual bool init();
		//CREATE_FUNC(MenuLayer);
		CREATE_WITH_WORLD(MenuLayer);

};

#endif
