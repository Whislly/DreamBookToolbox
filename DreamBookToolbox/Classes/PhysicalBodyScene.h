#ifndef __PhysicalBody_Layer_H__
#define __PhysicalBody_Layer_H__

#include "cocos2d.h"
#include "spine/spine-cocos2dx.h"

class PhysicalBodyLayer : public cocos2d::CCLayer
{
private:
    cocos2d::extension::CCSkeleton* skeletonNode;
public:
    PhysicalBodyLayer();
    ~PhysicalBodyLayer();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // a selector callback
    void menuReturnCallback(CCObject* pSender);

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    // implement the "static node()" method manually
    CREATE_FUNC(PhysicalBodyLayer);

    virtual void update( float delta );

    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
    virtual void ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
};

#endif  // __PhysicalBody_Layer_H__