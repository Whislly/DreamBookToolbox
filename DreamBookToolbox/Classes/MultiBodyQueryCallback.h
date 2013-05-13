#ifndef __MultiBodyQueryCallback_Callback_H_
#define __MultiBodyQueryCallback_Callback_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class MultiBodyQueryNode : public CCObject
{
public:
		b2Body *body;

		MultiBodyQueryNode(void) {}
};

class MultiBodyQueryCallback : public b2QueryCallback
{
public:
		b2Vec2 pos;
		CCArray *queryBodyArray;

		MultiBodyQueryCallback(b2Vec2 pos);
		~MultiBodyQueryCallback(void);

		virtual bool ReportFixture(b2Fixture* fixture);
};

#endif
