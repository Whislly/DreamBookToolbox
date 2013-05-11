#ifndef __PosQuery_Callback_H_
#define __PosQuery_Callback_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class PosQuery : public b2QueryCallback
{
public:
		b2Vec2 pos;
		b2Body* object;

		PosQuery(b2Vec2 pos);
		~PosQuery(void);

		virtual bool ReportFixture(b2Fixture* fixture);
};

#endif
