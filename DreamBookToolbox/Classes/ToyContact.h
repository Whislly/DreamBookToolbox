#ifndef __ToyContact_Contact_H_
#define __ToyContact_Contact_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class ToyContact : public b2ContactListener
{
public:
		ToyContact(void);
		~ToyContact(void);

		/// Called when two fixtures begin to touch.
		virtual void BeginContact(b2Contact* contact);

		/// Called when two fixtures cease to touch.
		virtual void EndContact(b2Contact* contact);

		virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
		//virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

};

#endif
