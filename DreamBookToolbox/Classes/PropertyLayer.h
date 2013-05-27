#ifndef __PropertyLayer_Layer_H__
#define __PropertyLayer_Layer_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"

#include "Define.h"

#include "KeyboardNotificationLayer.h"

USING_NS_CC;

class PropertyInput : public InputElement
{
public:
	CCSize inputRect;
	CCTextFieldTTF *text;
	CCSprite *sprite;
	CCSprite *inputSprite;
	PropertyType propertyType;

	PropertyInput(void);
	~PropertyInput();

	//base
	virtual bool init();
	CREATE_FUNC(PropertyInput);

	//base
	virtual CCRect GetRangeRect();
	virtual void OnEnter();
	virtual void OnLeave();
protected:
    cocos2d::CCObject*        m_leaveListener;
    cocos2d::SEL_MenuHandler  m_leaveSelector;
public:
    virtual void setSelectorForLeave(cocos2d::CCObject *target, cocos2d::SEL_MenuHandler leaveSelector);
};

class PropertyLayer : public KeyboardNotificationLayer
{
private:
	b2World *world;
public:
	PropertyLayer(void);
	~PropertyLayer(void);

	virtual bool init();
	virtual void draw();

	//CREATE_FUNC(PropertyLayer);
	CREATE_WITH_WORLD(PropertyLayer);
	void SetProperty(PropertyType type, void* data);
};

#endif
