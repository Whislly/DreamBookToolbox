#ifndef __PositionQueryCallback_Callback_H_
#define __PositionQueryCallback_Callback_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class PositionQueryCallback : public b2QueryCallback
{
public:
		b2Vec2 pos;
		b2Fixture* objFixture;

		PositionQueryCallback(b2Vec2 pos);
		~PositionQueryCallback(void);

		virtual bool ReportFixture(b2Fixture* fixture);
};

#endif
