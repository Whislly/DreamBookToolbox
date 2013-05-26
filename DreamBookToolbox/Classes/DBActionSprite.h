#ifndef __DBAction_Sprite_H__
#define __DBAction_Sprite_H__

#include "cocos2d.h"
#include "CCSpriteEx.h"
#include "DBData.h"
#include "DBPropertyData.h"
#include "Define.h"

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

    virtual void setVoiceEnabeld(bool isEnabled);
protected:
    void touchMovedDesign(cocos2d::CCTouch *pTouch);
    void transform(float time);
    void writeStatus(cocos2d::CCPoint pos, float time);
    void touchMovedAction(cocos2d::CCTouch *pTouch);
    void PressDelayEvent(cocos2d::CCObject* pSender);
public:
    void excuteAction(float time);
    void excuteAction(float time, cocos2d::CCObject *target, cocos2d::SEL_CallFunc selector);
    CC_SYNTHESIZE_READONLY(float, m_startTime, StartTime);
    CC_SYNTHESIZE_READONLY(float, m_endTime, EndTime);
    CC_SYNTHESIZE_READONLY(const char*, m_voiceContent, VoiceContent);
    void readStatus(float time);
protected:
    cocos2d::CCObject*       m_finishedActionListener;
    cocos2d::SEL_CallFunc    m_finishedActionSelector;
public:
    void save();
    void load();
    static DBActionSprite* create();
};

#endif  // __DBAction_Sprite_H__