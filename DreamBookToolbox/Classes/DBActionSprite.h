#ifndef __DBAction_Sprite_H__
#define __DBAction_Sprite_H__

#include "cocos2d.h"
#include "CCSpriteEx.h"
#include "DBData.h"
#include "DBPropertyData.h"

enum EnumStatus
{
    kDesignStatus,
    kDesignAction,
    kExcuteAction
};

class DBActionSprite : public cocos2d::CCSpriteEx
{
protected:
    void step(float time);
    float m_time;
    EnumStatus m_status;
    DBData* m_data;
public:
    DBActionSprite();
    ~DBActionSprite();
    static DBActionSprite* createWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect);
    static DBActionSprite* createWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect, int tag);

    void designStatus(float time);
    void designAction(float time);
    void clearAction(float time);
    void setEnableTime(bool isTime);

    virtual bool ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
    virtual void ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
    virtual void ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
    virtual void ccTouchCancelled( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
protected:
    void touchMovedDesign(cocos2d::CCTouch *pTouch);
    void readStatus(float time);
    void writeStatus(cocos2d::CCPoint pos, float time);
    void transform(float time);
    void touchMovedAction(cocos2d::CCTouch *pTouch);
public:
    void excuteAction(float time);
    CC_SYNTHESIZE_READONLY(float, m_startTime, StartTime);
    CC_SYNTHESIZE_READONLY(float, m_endTime, EndTime);
};

#endif  // __DBAction_Sprite_H__