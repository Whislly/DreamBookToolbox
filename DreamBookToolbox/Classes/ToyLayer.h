#ifndef __ToyLayer_Layer_H__
#define __ToyLayer_Layer_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"

#include "ToyBrick.h"
#include "ToyContact.h"
#include "PositionQueryCallback.h"

#include "CommonHelper.h"
#include "Define.h"

USING_NS_CC;

class ToyLayer : public cocos2d::CCLayer
{
private:
    b2World *world;
    b2Body *groundBody;

    b2MouseJoint *mouseJoint;

    CCArray *toyArray;

    CCTexture2D *boxTexture;
public:
    ToyLayer(void);
    ~ToyLayer();

    CREATE_FUNC(ToyLayer);

    virtual bool init();
    virtual void draw();
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

    void update(float dt);

    void AddToyBrick(ToyBrick *brick, CCPoint position);
    void AddRandomToyBrick(CCPoint position);
    ToyBrick* CreateBrick_Circle();
    ToyBrick* CreateBrick_Triangle();
    ToyBrick* CreateBrick_Rectangle();
    ToyBrick* CreateBrick_BigRectangle();

    virtual void keyBackClicked();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    virtual void processWin32KeyPress( UINT message, WPARAM wParam, LPARAM lParam );
    virtual void onEnter();
    virtual void onExit();
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
};

#endif
