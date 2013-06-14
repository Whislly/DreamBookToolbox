#ifndef __EFTestLayer_Layer_H__
#define __EFTestLayer_Layer_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"

#include "PositionQueryCallback.h"

#include "CommonHelper.h"
#include "Define.h"

#include "DockTogetherSpore.h"
#include "MainScene.h"

USING_NS_CC;

class EFTestLayer : public CCLayer
{
public:
	EFTestLayer(void);
	~EFTestLayer(void);

	virtual bool init();
	CREATE_FUNC(EFTestLayer);

	void menuBackCallback( CCObject* pSender );
};

#endif

