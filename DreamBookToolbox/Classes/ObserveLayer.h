#ifndef __Observe_Layer_H__
#define __Observe_Layer_H__

#include "cocos2d.h"
#include "ObserveLayerContainer.h"
#include "EnumObserverOrientation.h"

class ObserveLayerContainer;

class ObserveLayer : public cocos2d::CCLayerColor
{
protected:
    void drawBounder();
    void setCellStyle(cocos2d::CCSprite* cell);
    float m_cellsBound;
    ObserveLayerContainer* m_container;
    cocos2d::CCPoint m_tBeginPos;
    virtual bool init();
    ObserverOrientation m_orientation;
    /**
     * clip this view so that outside of the visible bounds can be hidden.
     */
    void beforeDraw();
    /**
     * retract what's done in beforeDraw so that there's no side effect to
     * other nodes.
     */
    void afterDraw();
    bool m_bClippingToBounds;
public:
    static ObserveLayer* create(const cocos2d::ccColor4B& color);
    void loadDefaultCell(cocos2d::CCSprite* defaultCell);
    void setOrientation(ObserverOrientation orientation);
    void highlightColor(int cellTag, const cocos2d::ccColor3B& color3);
    ObserverOrientation getOrientation();
    virtual void draw();
    void addCell(cocos2d::CCSprite* newCell);
    void removeCell(int cellTag, bool cleanup);

    cocos2d::CCSprite* getLastCell();

    ObserveLayer();
    ~ObserveLayer();

    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
    virtual void ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
    virtual void ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
    virtual void ccTouchCancelled( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
    virtual const char* description();
    virtual void visit( void );

    virtual void setContentSize( const cocos2d::CCSize & var );
};

#endif  // __Observe_Layer_H__