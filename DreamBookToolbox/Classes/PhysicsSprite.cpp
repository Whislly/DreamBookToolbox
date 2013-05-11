#include "PhysicsSprite.h"

#define PTM_RATIO 32

PhysicsSprite::PhysicsSprite(void)
{
		this->body = NULL;
}


PhysicsSprite::~PhysicsSprite(void)
{
}

void PhysicsSprite::setPhysicsBody(b2Body * theBody)
{
		this->body = theBody;
}

bool PhysicsSprite::isDirty(void)
{
		return true;
}

//CCAffineTransform PhysicsSprite::nodeToParentTransform(void)
//{
//		b2Vec2 pos  = this->body->GetPosition();
//
//		float x = pos.x * PTM_RATIO;
//		float y = pos.y * PTM_RATIO;
//
//		if ( this->isIgnoreAnchorPointForPosition() ) {
//				x += m_obAnchorPointInPoints.x;
//				y += m_obAnchorPointInPoints.y;
//		}
//
//		// Make matrix
//		float radians = this->body->GetAngle();
//		float c = cosf(radians);
//		float s = sinf(radians);
//
//		if( ! m_obAnchorPointInPoints.equals(CCPointZero) ){
//				x += c*-m_obAnchorPointInPoints.x + -s*-m_obAnchorPointInPoints.y;
//				y += s*-m_obAnchorPointInPoints.x + c*-m_obAnchorPointInPoints.y;
//		}
//
//		// Rot, Translate Matrix
//		m_tTransform = CCAffineTransformMake( c,  s,
//				-s,    c,
//				x,    y );
//
//		return m_tTransform;
//}

CCAffineTransform PhysicsSprite::nodeToParentTransform(void)
{
		b2Vec2 pos  = this->body->GetPosition();

		float x = pos.x * PTM_RATIO;
		float y = pos.y * PTM_RATIO;

		if ( this->isIgnoreAnchorPointForPosition() ) {
				x += m_obAnchorPointInPoints.x;
				y += m_obAnchorPointInPoints.y;
		}

		// Make matrix
		float radians = this->body->GetAngle();
		float c = cosf(radians);
		float s = sinf(radians);

		if( ! m_obAnchorPointInPoints.equals(CCPointZero) ){
				x += c*-m_obAnchorPointInPoints.x + -s*-m_obAnchorPointInPoints.y;
				y += s*-m_obAnchorPointInPoints.x + c*-m_obAnchorPointInPoints.y;
		}

		// Rot, Translate Matrix
		m_transformToBatch = CCAffineTransformMake( c,  s,
				-s,    c,
				x,    y );

		return m_transformToBatch;
}