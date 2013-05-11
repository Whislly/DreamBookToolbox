#ifndef __KeyboardNotificationLayer_Layer_H__
#define __KeyboardNotificationLayer_Layer_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"

#include "Define.h"

#include "CommonHelper.h"

USING_NS_CC;

class InputElement : public CCNode
{
public:
		virtual CCRect GetRangeRect() = 0;
		virtual void OnEnter() = 0;
		virtual void OnLeave() = 0;
};


class KeyboardNotificationLayer : public CCLayer, public CCIMEDelegate
{
private:
		CCPoint beginPos;
		InputElement *current;
public:
		CCArray *nodeArray;

		KeyboardNotificationLayer(void);
		~KeyboardNotificationLayer(void);

		virtual void registerWithTouchDispatcher();
		virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info);

		// CCLayer
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};

#endif
