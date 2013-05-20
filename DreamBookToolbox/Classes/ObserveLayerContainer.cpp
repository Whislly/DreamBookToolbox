#include "ObserveLayerContainer.h"
#include "Define.h"

#define DefaultCellTag  0
#define Offset          2

USING_NS_CC;

ObserveLayerContainer* ObserveLayerContainer::create(const ccColor4B& color)
{
    ObserveLayerContainer* pContainer = new ObserveLayerContainer();
    pContainer->initWithColor(color);
    pContainer->autorelease();
    return pContainer;
}

void ObserveLayerContainer::calcChildPosition( cocos2d::CCSprite* cell )
{
    CCSize cellSize = cell->getContentSize();
    CCSize contentSize = getContentSize();
    float actualPixes = 0.0f;
    float scale = 1.0f;

    if (m_direction == kOrientationHor)
    {
        actualPixes = contentSize.height - Offset * 2;
        if (actualPixes < cellSize.height)
        {
            scale = actualPixes / cellSize.height;
        }
        cell->setScale(scale);
        cell->setPosition(ccp(m_startPos.x + cellSize.width * scale * 0.5f, contentSize.height * 0.5f));
        m_startPos = ccpAdd(m_startPos, ccp(cellSize.width * scale + Offset * 2, 0));
    }
    else
    {
        actualPixes = contentSize.width - Offset * 2;
        if (actualPixes < cellSize.width)
        {
            scale = actualPixes / cellSize.width;
        }
        cell->setScale(scale);
        cell->setPosition(ccp(contentSize.width * 0.5f, m_startPos.y - cellSize.height * scale * 0.5f));
        m_startPos = ccpSub(m_startPos, ccp(0, cellSize.height * scale + Offset * 2));
    }
}

void ObserveLayerContainer::removeCell( int cellTag, bool cleanup )
{
    this->removeChildByTag(cellTag, cleanup);
}

cocos2d::CCSprite* ObserveLayerContainer::getLastCell()
{
    return (cocos2d::CCSprite*)getChildByTag(getChildrenCount());
}

void ObserveLayerContainer::addCell( cocos2d::CCSprite* newCell )
{
    this->addChild(newCell, 0, this->getChildrenCount());
    this->calcChildPosition(newCell);
    newCell->runAction(CCSequence::create(CCFadeIn::create(0.4f), NULL));
}

void ObserveLayerContainer::loadDefaultCell( cocos2d::CCSprite* defaultCell )
{
    this->addChild(defaultCell, 0, DefaultCellTag);
    this->calcChildPosition(defaultCell);
    defaultCell->runAction(CCSequence::create(CCFadeIn::create(0.4f), NULL));
}

void ObserveLayerContainer::setOrientation( ObserverOrientation orientation )
{
    m_direction = orientation;

    CCSize contentSize = getContentSize();
    switch(m_direction)
    {
    case kOrientationHor:
        m_startPos = ccp(Offset * 2, contentSize.height * 0.5f);
        break;
    case kOrientationVer:
        m_startPos = ccp(contentSize.width * 0.5f, contentSize.height - Offset * 2);
        break;
    }
}

void ObserveLayerContainer::highlightColor( int cellTag, const cocos2d::ccColor3B& color3 )
{
    if (m_oldHighlightCell)
    {
        //m_oldHighlightCell->setColor(ccWHITE);
        m_oldHighlightCell->stopActionByTag(FocusActionTag);
        m_oldHighlightCell->setScale(m_oldHighlightCellScaleValue);
    }
    CCSprite* pChild = (CCSprite*)(this->getChildByTag(cellTag));
    //pChild->setColor(color3);
    m_oldHighlightCellScaleValue = pChild->getScale();
    CCActionInterval* pAction = CCRepeatForever::create(
        (CCActionInterval*)CCSequence::create(CCScaleTo::create(0.4f, m_oldHighlightCellScaleValue * 1.1f), 
         CCScaleTo::create(0.4f, m_oldHighlightCellScaleValue * 0.9f), NULL)
         );
    pChild->runAction(pAction);
    pAction->setTag(FocusActionTag);
    m_oldHighlightCell = pChild;
}

ObserverOrientation ObserveLayerContainer::getOrientation()
{
    return m_direction;
}

ObserveLayerContainer::ObserveLayerContainer()
    : m_direction(kOrientationVer)
    , m_startPos(CCPointZero)
    , m_oldHighlightCell(NULL)
    , m_oldHighlightCellScaleValue(1.0f)
{

}

ObserveLayerContainer::~ObserveLayerContainer()
{
    m_oldHighlightCell = NULL;
}