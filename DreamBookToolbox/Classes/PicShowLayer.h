#ifndef __PicShowLayer_Layer_H__
#define __PicShowLayer_Layer_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"
#include "CommonHelper.h"
#include "Define.h"

#include "IInputNotifier.h"

#include "FileSprite.h"

USING_NS_CC;


class PicShowLayer : public CCLayerColor
{
private:
	CCSprite* currentSelect;
public:
	int startIndex;
	//int elementCount;
	CCArray *elementArr;

	PicShowLayer(void);
	~PicShowLayer(void);

	static PicShowLayer* create(int startIndex);

	virtual bool init();
	virtual void draw();
	void update(float dt);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
};

#endif
