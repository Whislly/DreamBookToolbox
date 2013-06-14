#ifndef __DancingBoneScene_SCENE_H__
#define __DancingBoneScene_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"

#include "PositionQueryCallback.h"

#include "CommonHelper.h"
#include "Define.h"

#include "DancingBoneLayer.h"
#include "DancingBoneMenuLayer.h"

USING_NS_CC;

class DancingBoneScene : public CCScene
{
public:
	DancingBoneScene(void);
	~DancingBoneScene(void);

	virtual bool init();
	CREATE_FUNC(DancingBoneScene);
};

#endif
