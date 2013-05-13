#ifndef __CCSPRITEEX_H__
#define __CCSPRITEEX_H__

// CCSpriteEx 是对 CCSprite类的扩展，主要是能得到触摸事件的反馈

#include <cocos2d.h>
#include <cocos-ext.h>

NS_CC_BEGIN

class CCSpriteEx
    : public CCSprite
    , public CCTargetedTouchDelegate
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CCSpriteEx, create); // 用于CocosBuilder

    static CCSpriteEx* create(const char* pszFileName);
    static CCSpriteEx* createWithSpriteFrameName(const char* pszFrameName);
    /** Creates an sprite with an sprite frame. */
    static CCSpriteEx* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
    static CCSpriteEx* createWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect);

    static inline long millisecondNow()
    {
        struct cc_timeval now;
        CCTime::gettimeofdayCocos2d(&now, NULL);
        return (now.tv_sec * 1000 + now.tv_usec / 1000);
    }
public:
    CCSpriteEx();

private:
    void onEnter();
    void onExit();
    // 取得精灵的位置与尺寸
    CCRect rect();
    // 触摸点是否在精灵上
    bool isTouchInside(CCTouch* touch);
    void restoreStatus();

protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    // optional
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

    virtual void ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent );
    virtual void ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent );
    virtual void ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent );
    virtual void ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent );

private:
    float m_originScale;

    long m_lastTouchTime;
    bool m_isInTouch;
    bool m_isInMove;
    bool m_afterLongPress;
    void checkLongPress(float touchTime);
    inline bool isDoubleTouch()
    {
        long thisTouchTime = millisecondNow();
        if(abs(thisTouchTime - m_lastTouchTime) < 250)
        {
            m_lastTouchTime = 0;
            return true;
        }
        else
        {
            m_lastTouchTime = millisecondNow();
            return false;
        }
    }
public:
    virtual CCObject* copy();
    virtual void setSelectorForSingleClick(CCObject *target, SEL_MenuHandler singleClickSelector);
    virtual void setSelectorForDoubleClick(CCObject *target, SEL_MenuHandler doubleClickSelector);
    virtual void setSelectorForLongClick(CCObject *target, SEL_MenuHandler longClickSelector);
    virtual void setSelectorForScale(CCObject* target, SEL_MenuHandler scaleSelector);
    CC_SYNTHESIZE(bool, m_scaleEnabled, ScaleEnabled);
    CC_SYNTHESIZE(bool, m_singleClickEnabled, SingleClickEnabled);
    CC_SYNTHESIZE(bool, m_doubleClickEnabled, DoubleClickEnabled);
    CC_SYNTHESIZE(bool, m_longClickEnabled, LongClickEnabled);
protected:
    CCObject*       m_scaleListener;
    SEL_MenuHandler m_scaleSelector;
    float           m_distance;

    // 点击事件处理
    CCObject*       m_longClickListener;
    SEL_MenuHandler m_longClickSelector;

    CCObject*       m_singleClickListener;
    SEL_MenuHandler m_singleClickSelector;

    CCObject*       m_doubleClickListener;
    SEL_MenuHandler m_doubleClickSelector;
    float calcDistance(cocos2d::CCTouch *pTouch);

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    virtual void processWin32KeyPress( UINT message, WPARAM wParam, LPARAM lParam );
    #endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

    float calcDistance(CCSet *pTouches);
};

class CCSpriteExLoader : public cocos2d::extension::CCSpriteLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCSpriteExLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCSpriteEx);
};

NS_CC_END 
#endif // __CCSPRITEEX_H__