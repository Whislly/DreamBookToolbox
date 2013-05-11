#ifndef __DynamicLayerContainer_Container_H__
#define __DynamicLayerContainer_Container_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"
#include "CommonHelper.h"
#include "Define.h"

#include "PicShowLayer.h"

#include "IInputNotifier.h"

USING_NS_CC;


class DynamicLayerContainer : public CCNode, public IInputNotifier
{
private:
		int totalCount;
		int currentLayerIndex;
		CCArray *arr;
public:
		DynamicLayerContainer(void);
		~DynamicLayerContainer(void);

		static DynamicLayerContainer* create(int totalCount);

		virtual bool init();

		virtual void ClickOnSprite(CCPoint location, CCSprite *sprite);

		PicShowLayer* GetCurrentShowLayer();
		PicShowLayer* GoToNext();
		PicShowLayer* GoToPre();
};

#endif


