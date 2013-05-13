#include "MultiBodyQueryCallback.h"

MultiBodyQueryCallback::MultiBodyQueryCallback(b2Vec2 pos)
{
		this->queryBodyArray = CCArray::create();
		this->queryBodyArray->retain();

		this->pos = pos;
}

MultiBodyQueryCallback::~MultiBodyQueryCallback(void)
{
		if (this->queryBodyArray != NULL)
		{
				this->queryBodyArray->release();
		}
}

bool MultiBodyQueryCallback::ReportFixture(b2Fixture* fixture)
{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
				bool inside = fixture->TestPoint(this->pos);
				if (inside)
				{
						MultiBodyQueryNode *node = new MultiBodyQueryNode();
						node->autorelease();
						node->body = fixture->GetBody();
						this->queryBodyArray->addObject(node);

						//countinue;
				}
		}

		// Continue the query.
		return true;
}
