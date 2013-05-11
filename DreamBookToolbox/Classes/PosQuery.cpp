#include "PosQuery.h"


PosQuery::PosQuery(b2Vec2 pos)
{
		this->object = NULL;
		this->pos = pos;
}

PosQuery::~PosQuery(void)
{
}

bool PosQuery::ReportFixture(b2Fixture* fixture)
{
		b2Body* body = fixture->GetBody();
		//if (body->GetType() == b2_dynamicBody)
		//{
				bool inside = fixture->TestPoint(this->pos);
				if (inside)
				{
						this->object = fixture->GetBody();

						// We are done, terminate the query.
						return false;
				}
		//}

		// Continue the query.
		return true;
}
