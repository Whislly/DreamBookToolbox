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

		// Although scale is not used by physics engines, it is calculated just in case
		// the sprite is animated (scaled up/down) using actions.
		// For more info see: http://www.cocos2d-iphone.org/forum/topic/68990
		if (!m_obAnchorPointInPoints.equals(CCPointZero))
		{
			x += ((c * -m_obAnchorPointInPoints.x * m_fScaleX) + (-s * -m_obAnchorPointInPoints.y * m_fScaleY));
			y += ((s * -m_obAnchorPointInPoints.x * m_fScaleX) + (c * -m_obAnchorPointInPoints.y * m_fScaleY));
		}

		// Rot, Translate Matrix
		m_sTransform = CCAffineTransformMake( c * m_fScaleX,	s * m_fScaleX,
			-s * m_fScaleY,	c * m_fScaleY,
			x,	y );

		return m_sTransform;
}