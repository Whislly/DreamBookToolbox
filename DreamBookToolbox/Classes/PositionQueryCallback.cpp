#include "PositionQueryCallback.h"


PositionQueryCallback::PositionQueryCallback(b2Vec2 pos)
{
		this->objFixture = NULL;
		this->pos = pos;
}

PositionQueryCallback::~PositionQueryCallback(void)
{
}

bool PositionQueryCallback::ReportFixture(b2Fixture* fixture)
{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
				bool inside = fixture->TestPoint(this->pos);
				if (inside)
				{
						this->objFixture = fixture;

						// We are done, terminate the query.
						return false;
				}
		}

		// Continue the query.
		return true;
}
