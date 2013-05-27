#ifndef __Story_Layer_H__
#define __Story_Layer_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

class StoryLayer : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    void addCaption(cocos2d::CCNode* pSender);

    void startStory(cocos2d::CCObject* pSender);

    void loadChat(cocos2d::CCNode* pSender);

    // a selector callback
    void menuReturnCallback(CCObject* pSender);

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    // implement the "static node()" method manually
    CREATE_FUNC(StoryLayer);
};

#endif  // __Story_Layer_H__