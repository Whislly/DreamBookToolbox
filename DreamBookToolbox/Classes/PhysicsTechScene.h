#ifndef __PhysicsTech_SCENE_H__
#define __PhysicsTech_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"

#include "PositionQueryCallback.h"

#include "CommonHelper.h"
#include "Define.h"

#include "MenuLayer.h"
#include "PropertyLayer.h"
#include "PhysicsTechLayer.h"

USING_NS_CC;


class PhysicsTechScene : public cocos2d::CCScene
{
public:
	PhysicsTechScene(void);
	~PhysicsTechScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static node()" method manually
    CREATE_FUNC(PhysicsTechScene);
};

#endif  // __PhysicsTech_SCENE_H__