#ifndef __DancingBoneLayer_Layer_H__
#define __DancingBoneLayer_Layer_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"

#include "PositionQueryCallback.h"
#include "MultiBodyQueryCallback.h"

#include "CommonHelper.h"
#include "Define.h"

#include "ToyBrick.h"

USING_NS_CC;

class MouseJointObject : public CCObject
{
public :
	b2MouseJoint *mouseJoint;
};

class DancingBoneLayer : public CCLayer
{
private:		
    b2World *world;
    b2Body *groundBody;

    //b2MouseJoint *mouseJoint;
    CCArray *boneArray;

	CCMenuItemImage *pushpin;

	bool isReal;
	bool isPushpinDown;

	CCSprite *pushpinNormal;
	CCSprite *pushpinDown;

	CCDictionary *touchDic;

public:
    DancingBoneLayer(void);
    ~DancingBoneLayer();

    CREATE_FUNC(DancingBoneLayer);

    virtual bool init();
    virtual void draw();
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

    void update(float dt);

	void InitBones();
	void InitButtonItem();

	//Events
	void OnPushpinClick(CCObject* pSender);

	ToyBrick* CreateBrick_Circle();
	ToyBrick* CreateBrick_Triangle();
	ToyBrick* CreateBrick_Rectangle();
	ToyBrick* CreateBrick_BigRectangle();
	void AddToyBrick(ToyBrick *brick, CCPoint position);

	ToyBrick* CreateHead();
	ToyBrick* CreateBody();
	ToyBrick* CreateArm();

	void SetMode(bool isReal);

};

#endif

