#ifndef __PicSelectionLayer_Layer_H__
#define __PicSelectionLayer_Layer_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"
#include "CommonHelper.h"
#include "Define.h"

#include "PicShowLayer.h"
#include "PicPickupLayer.h"

#include "GestureLayer.h"

#include "CCSpriteEx.h"

#include "FileSprite.h"
#include "FrameSprite.h"

USING_NS_CC;

class PicSelectionLayer : public CCLayerColor
{
private:
		CCArray *elementArray;
		CCArray *selectArray;
		CCSprite *currentSelect;
public:
		PicSelectionLayer(void);
		~PicSelectionLayer(void);

		CREATE_FUNC(PicSelectionLayer);

		virtual bool init();
		virtual void draw();
		void update(float dt);

		virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
		virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
		virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
		virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

		CCPoint GetAdjustPos(CCSize spriteSize, int currentIndex);
		CCPoint GetPreAddPos(CCSize spriteSize);
		void AddElement(CCSprite *sprite);

		FrameSprite* CombineSpriteOnTime(CCArray *spriteArray, float interval);
		void AdjustSprite(CCArray *spriteArray, CCSprite *newSprite);

		void ClickOnCombineTime(CCObject* pSender);

        CCArray* getElementArray();
};

#endif
