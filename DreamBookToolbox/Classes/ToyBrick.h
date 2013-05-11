#ifndef __ToyBrick_Sprite_H__
#define __ToyBrick_Sprite_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"

#include "Define.h"

USING_NS_CC;


class ToyBrick : public PhysicsSprite
{
public:
		CCPointArray *vecArray;
		ShapeType type;
		int radius;

		ToyBrick(void);
        ~ToyBrick();

		CREATE_FUNC(ToyBrick);

		void Init(CCTexture2D* texture, CCRect textureRect);
		void SetAsPolygon(CCPointArray* vecArray);
		void SetAsCircle(int radius);
};

#endif
