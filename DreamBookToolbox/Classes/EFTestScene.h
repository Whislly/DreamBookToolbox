#ifndef __EFTestScene_SCENE_H__
#define __EFTestScene_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"

#include "PositionQueryCallback.h"

#include "CommonHelper.h"
#include "Define.h"

#include "EFTestLayer.h"

USING_NS_CC;

class EFTestScene : public CCScene
{
public:
	EFTestScene(void);
	~EFTestScene();

	virtual bool init();
	CREATE_FUNC(EFTestScene);
};

#endif
