#ifndef __Gesture_Layer_H__
#define __Gesture_Layer_H__

#include "GeometricRecognizer.h"
#include "GeometricRecognizerTypes.h"

#include "cocos2d.h"

typedef void (cocos2d::CCObject::*SEL_GestureHandler)(const char* gestureName, double score);
#define gesture_selector(_SELECTOR) (SEL_GestureHandler)(&_SELECTOR)

class GestureLayer : public cocos2d::CCLayer
{
private:
	DollarRecognizer::Path2D m_2dPath;
	DollarRecognizer::GeometricRecognizer* m_recognizer;
	SEL_GestureHandler m_selector;	
	cocos2d::CCNode* m_target;
	cocos2d::CCParticleSystem*    m_emitter;
	void init(cocos2d::CCParticleSystem* particle);
	void moveParticle(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
public:
	static GestureLayer* create(cocos2d::CCParticleSystem* particle);
    void setGestureHandler(cocos2d::CCNode* pTarget, SEL_GestureHandler selector);
	~GestureLayer();

    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
    virtual void ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
    virtual void ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
};

#endif // __Gesture_Layer_H__