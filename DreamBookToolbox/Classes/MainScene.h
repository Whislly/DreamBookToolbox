#ifndef __Main_SCENE_H__
#define __Main_SCENE_H__

#include "cocos2d.h"

class MainLayer : public cocos2d::CCLayer
{
public:
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene(); 

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // a selector callback
    void menuDreamBookCallback(CCObject* pSender);

    // a selector callback
    void menuPhysicsTechCallback(CCObject* pSender);

    // a selector callback
    void menuToyBrickCallback(CCObject* pSender);

    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(MainLayer);
};

#endif  // __Main_SCENE_H__