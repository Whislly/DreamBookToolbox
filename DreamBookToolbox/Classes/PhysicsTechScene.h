#ifndef __PhysicsTech_SCENE_H__
#define __PhysicsTech_SCENE_H__

#include "cocos2d.h"

class PhysicsTechScene : public cocos2d::CCScene
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static node()" method manually
    CREATE_FUNC(PhysicsTechScene);
};

#endif  // __PhysicsTech_SCENE_H__