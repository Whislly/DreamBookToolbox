#ifndef __PicPickupLayer_Layer_H__
#define __PicPickupLayer_Layer_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "VisibleRect.h"
#include "PhysicsSprite.h"
#include "CommonHelper.h"
#include "Define.h"

#include "PicShowLayer.h"
#include "DynamicLayerContainer.h"
#include "PicSelectionLayer.h"

#include "IInputNotifier.h"

USING_NS_CC;

class PicSelectionLayer;

class PicPickupLayer : public CCLayer, public IInputNotifier
{
private:
		DynamicLayerContainer *container;
		PicSelectionLayer *selectionLayer;

		void RemoveSportSpriteEvent(CCNode* sender, void* data);
		void TransferSpriteEvent(CCNode* sender, void* data);
public:
		PicPickupLayer(void);
		~PicPickupLayer(void);

		CREATE_FUNC(PicPickupLayer);

		virtual bool init();
		virtual void draw();
		void update(float dt);
		
		virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
		virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
		virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
		virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

        virtual void ClickOnSprite(CCPoint location, CCSprite *sprite);

        CCArray* getElementArray();
        void load();
protected:
    CCObject*       m_pListener;
    SEL_ImportElementsHandler m_importElementsSelector;
public:
    // 设置点击精灵的事件处理器
    void setSelectorWithTarget(CCObject *target, SEL_ImportElementsHandler importElementsSelector);
    static cocos2d::CCScene* scene(CCObject *target, SEL_ImportElementsHandler importElementsSelector);
    // a selector callback
    void menuBackCallback(CCObject* pSender);
};

#endif

