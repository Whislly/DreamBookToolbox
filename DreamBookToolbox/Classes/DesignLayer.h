#ifndef __DesignLayer_H__
#define __DesignLayer_H__

#include "cocos2d.h"
#include "CCSpriteEx.h"

class DesignLayer : public cocos2d::CCLayerColor
{
public:
    bool isContainsChild(int tag);
    void addSpriteEx(cocos2d::CCSpriteEx* pSprite);
    void addSpriteEx(cocos2d::CCSpriteEx* pSprite, int zOrder);
    void removeSpriteEx(int tag, bool cleanup);
    static DesignLayer* create(const cocos2d::ccColor4B& color);

    void designSpriteStatus(int tag);
    void activeActions(int tag);
    void setEnableTime(bool isTime);

    DesignLayer();
    ~DesignLayer();

    void addTimeLabel();

    void runChildrenActions();
protected:
    float m_time;
    void step(float time);
    cocos2d::CCLabelBMFont* m_lblTime;
    int m_currentSpriteTag;
    void stopChildrenActions(int tag);
    void runChildrenActions(int tag);
    void focusChild(cocos2d::CCSprite* pSprite);
    int m_finishedActionCount;
private:
    void addFinishedActionCount();
    cocos2d::CCSpriteEx*       m_enlarge;
    cocos2d::CCSpriteEx*       m_reduce;
    cocos2d::CCSprite*         m_currentSprite;
    void enlarge(cocos2d::CCObject* pSender);
    void reduce(cocos2d::CCObject* pSender);
    void showScaleToolButtons(cocos2d::CCObject* pSender);

    virtual bool ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
    virtual void onEnter();
    virtual void onExit();
public:
    void saveData();
};

#endif  // __DesignLayer_H__