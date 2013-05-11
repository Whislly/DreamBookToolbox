#ifndef __DesignLayer_H__
#define __DesignLayer_H__

#include "cocos2d.h"

class DesignLayer : public cocos2d::CCLayerColor
{
public:
    bool isContainsChild(int tag);
    void addSpriteEx(cocos2d::CCSprite* pSprite);
    void addSpriteEx(cocos2d::CCSprite* pSprite, int zOrder);
    void removeSpriteEx(int tag, bool cleanup);
    static DesignLayer* create(const cocos2d::ccColor4B& color);

    void designSpriteStatus(int tag);
    void activeActions(int tag);
    void setEnableTime(bool isTime);

    DesignLayer();
    ~DesignLayer();

    void addTimeLabel();
protected:
    float m_time;
    void step(float time);
    cocos2d::CCLabelBMFont* m_lblTime;
    int m_currentSpriteTag;
    void stopChildrenActions(int tag);
    void runChildrenActions(int tag);
    void focusChild(cocos2d::CCSprite* pSprite);
};

#endif  // __DesignLayer_H__