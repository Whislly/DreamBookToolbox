#ifndef __ObserveLayer_Container_H__
#define __ObserveLayer_Container_H__

#include "cocos2d.h"
#include "ObserveLayer.h"
#include "EnumObserverOrientation.h"

class ObserveLayerContainer : public cocos2d::CCLayerColor
{
private:
    ObserverOrientation m_direction;
    static ObserveLayerContainer* create(const cocos2d::ccColor4B& color);
    cocos2d::CCPoint m_startPos;
    void calcChildPosition(cocos2d::CCSprite* cell);
    cocos2d::CCSprite* m_oldHighlightCell;
    float m_oldHighlightCellScaleValue;
protected:
    void loadDefaultCell(cocos2d::CCSprite* defaultCell);
    void addCell(cocos2d::CCSprite* newCell);
    void removeCell(int cellTag, bool cleanup);
    cocos2d::CCSprite* getLastCell();
    void setOrientation(ObserverOrientation orientation);
    void highlightColor(int cellTag, const cocos2d::ccColor3B& color3);
    ObserverOrientation getOrientation();
    ObserveLayerContainer();
    ~ObserveLayerContainer();
public:
    friend class ObserveLayer;
};

#endif  // __ObserveLayer_Container_H__