#ifndef __PhysicsSprite_Sprite_H_
#define __PhysicsSprite_Sprite_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class PhysicsSprite : public CCSprite
{
public:
		b2Body* body;    // strong ref

		PhysicsSprite(void);
		~PhysicsSprite(void);

		CREATE_FUNC(PhysicsSprite);

		void setPhysicsBody(b2Body * body);
		virtual CCAffineTransform nodeToParentTransform(void);
		virtual bool isDirty(void);
};

#endif
