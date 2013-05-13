#ifndef __DancingBoneMenuLayer_Layer_H__
#define __DancingBoneMenuLayer_Layer_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"

#include "PositionQueryCallback.h"

#include "CommonHelper.h"
#include "Define.h"

#include "DancingBoneLayer.h"
#include "DancingBoneScene.h"

#include "MainScene.h"

USING_NS_CC;

class DancingBoneMenuLayer : public CCLayer
{		
private:
		bool isRealMode;
		CCMenuItemImage *startPauseItem;
		CCSprite *startImage;
		CCSprite *pauseImage;
public:
		DancingBoneLayer *mainLayer;

		DancingBoneMenuLayer(void);
		~DancingBoneMenuLayer();

		CREATE_FUNC(DancingBoneMenuLayer);

		virtual bool init();
		virtual void draw();

		void update(float dt);

		void StartPause(CCObject* pSender);
		void Reset(CCObject* pSender);
		void ReturnBack(CCObject* pSender);
};

#endif

