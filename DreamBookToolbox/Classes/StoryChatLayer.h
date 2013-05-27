#ifndef __StoryChat_Layer_H__
#define __StoryChat_Layer_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

class StoryChatLayer : public cocos2d::CCLayer
{
private:
    cocos2d::CCSprite* m_olderBrother;
    cocos2d::CCSprite* m_kongrong;
    cocos2d::CCSprite* m_youngBrother;

    cocos2d::CCSprite* m_bigPear;
    cocos2d::CCSprite* m_middlePear;
    cocos2d::CCSprite* m_smallPear;
    cocos2d::CCSprite* m_movingPear;

    cocos2d::CCSprite* m_feeling;
    bool m_correctBrothers[3];

    void findPear(cocos2d::CCPoint localPos, cocos2d::CCSprite* pear);
    bool hasGotPear(cocos2d::CCPoint localPos, cocos2d::CCSprite* brother);
    void smile(cocos2d::CCSprite* brother);
    void surprise(cocos2d::CCSprite* brother);
    cocos2d::CCLabelTTF* m_caption;
    void resetPosition(unsigned int i);
public:
    StoryChatLayer();
    ~StoryChatLayer();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    void chat(CCNode* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(StoryChatLayer);

    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
    virtual void ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
    virtual void ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
};

#endif  // __StoryChat_Layer_H__