#ifndef __PhysicsTechLayer_Sprite_H__
#define __PhysicsTechLayer_Sprite_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"
#include "CustomUserData.h"
#include "PosQuery.h"
#include "PropertyLayer.h"

#include "Define.h"

USING_NS_CC;

class PhysicsTechLayer : public CCLayer
{
protected:
		b2Body *currentSelection;

		void GetPropertyAndShow(b2Body *object, PropertyLayer *layer);
		void SetProperty(PropertyLayer *layer, b2Body *object);
public:
		b2World *world;
		b2Body *groundBody;
		PropertyLayer *propertyLayer;

		PhysicsTechLayer(void);
		~PhysicsTechLayer(void);

		//CREATE_FUNC(PhysicsTechLayer)
		CREATE_WITH_WORLD(PhysicsTechLayer);

		virtual bool init();
		virtual void draw();
		
		virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
		virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
		virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
		virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

		void update(float dt);
};

#endif
