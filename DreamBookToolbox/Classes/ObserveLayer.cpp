#include "ObserveLayer.h"

USING_NS_CC;


void ObserveLayer::setOrientation( ObserverOrientation orientation )
{
    if (m_container)
    {
        m_container->setOrientation(orientation);
    }
}

ObserverOrientation ObserveLayer::getOrientation()
{
    ObserverOrientation orientation = kOrientationVer;
    if (m_container)
    {
        orientation = m_container->getOrientation();
    }
    return orientation;
}

ObserveLayer* ObserveLayer::create(const ccColor4B& color)
{
    ObserveLayer* pLayer = new ObserveLayer();
    pLayer->initWithColor(color);
    pLayer->autorelease();
    pLayer->m_container = ObserveLayerContainer::create(color);
    pLayer->init();
    return pLayer;
}

void ObserveLayer::addCell( cocos2d::CCSprite* newCell )
{
    if (m_container)
    {
        m_container->addCell(newCell);
    }
}

void ObserveLayer::removeCell( int cellTag, bool cleanup )
{
    if (m_container)
    {
        m_container->removeCell(cellTag, cleanup);
    }
}

void ObserveLayer::loadDefaultCell(cocos2d::CCSprite* defaultCell)
{
    if (m_container)
    {
        m_container->loadDefaultCell(defaultCell);
    }
}

void ObserveLayer::highlightColor( int cellTag, const cocos2d::ccColor3B& color3 )
{
    if (m_container)
    {
        m_container->highlightColor(cellTag, color3);
    }
}

void ObserveLayer::draw()
{
    CCLayerColor::draw();
    this->drawBounder();
}

void ObserveLayer::drawBounder()
{
    glLineWidth( 1.0f );
    ccDrawColor4B(255 - _displayedColor.r, 255 - _displayedColor.g, 255 - _displayedColor.b, 255);
    CCSize contentSize = getContentSize();
    ccDrawRect(ccp(contentSize.width - 1.0f, contentSize.height - 1.0f), ccp(2.0f, 1.0f));
}

bool ObserveLayer::init()
{
    m_container->retain();
    m_container->ignoreAnchorPointForPosition(false);
    m_container->setAnchorPoint(CCPointZero);
    m_container->setPosition(CCPointZero);
    this->addChild(m_container);
    return true;
}

ObserveLayer::ObserveLayer()
    : m_container(NULL) 
    , m_cellsBound(0.0f)
    , m_orientation(kOrientationVer)
    , m_tBeginPos(CCPointZero)
    , m_bClippingToBounds(true)
{

}

ObserveLayer::~ObserveLayer()
{
    if (m_container)
    {
        m_container->release();
    }
    m_container = NULL;
}

void ObserveLayer::onEnter()
{
    CCLayer::onEnter();
    setTouchEnabled(true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void ObserveLayer::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    setTouchEnabled(false);
}

bool ObserveLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    if (!this->isVisible())
    {
        return false;
    }
    CCRect rect = boundingBox();
    CCPoint touchPoint = pTouch->getLocation();
    if (rect.containsPoint(touchPoint))
    {
        m_tBeginPos = touchPoint;
        return true;
    }
    return false;
}

void ObserveLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
    if (!this->isVisible())
    {
        return;
    }
    CCPoint touchLocation = pTouch->getLocation();
    CCPoint nextPos = m_container->getPosition();
    float maxOffset = 0.0f;
    if (m_orientation == kOrientationHor)
    {
        maxOffset = 0.0f - m_container->m_startPos.x + getContentSize().width;
        nextPos.x = touchLocation.x - m_tBeginPos.x + nextPos.x;
        if (nextPos.x < maxOffset)
        {
            nextPos.x = maxOffset;
        }
        if (nextPos.x > 0.0f)
        {
            nextPos.x = 0.0f;
        }
    }
    else
    {
        maxOffset = 0 - m_container->m_startPos.y;
        nextPos.y = touchLocation.y - m_tBeginPos.y + nextPos.y;
        if (nextPos.y > maxOffset)
        {
            nextPos.y = maxOffset;
        }
        if (nextPos.y < 0.0f)
        {
            nextPos.y = 0.0f;
        }
    }
    m_container->setPosition(nextPos);
    m_tBeginPos = touchLocation;
}

void ObserveLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
    if (!this->isVisible())
    {
        return;
    }
}

void ObserveLayer::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void ObserveLayer::setContentSize( const CCSize & var )
{
    CCLayerColor::setContentSize(var);
    if(m_container)
        m_container->setContentSize(var);
}

const char* ObserveLayer::description()
{
    return "ObserveLayer";
}

//////////////////////////////////////////////////////////////////////////
///Clip Region
//////////////////////////////////////////////////////////////////////////
void ObserveLayer::beforeDraw()
{
    if (m_bClippingToBounds)
    {
        CCSize viewSize = CCLayer::getContentSize();
        // TODO: This scrollview should respect parents' positions
        CCPoint screenPos = this->getParent()->convertToWorldSpace(this->getPosition());

        glEnable(GL_SCISSOR_TEST);
        float s = this->getScale();

        //        CCDirector *director = CCDirector::sharedDirector();
        //        s *= director->getContentScaleFactor();
        CCEGLView::sharedOpenGLView()->setScissorInPoints(screenPos.x*s, screenPos.y*s, viewSize.width*s, viewSize.height*s);
        //glScissor((GLint)screenPos.x, (GLint)screenPos.y, (GLsizei)(m_tViewSize.width*s), (GLsizei)(m_tViewSize.height*s));

    }
}

void ObserveLayer::afterDraw()
{
    if (m_bClippingToBounds)
    {
        glDisable(GL_SCISSOR_TEST);
    }
}

void ObserveLayer::visit( void )
{
    // quick return if not visible
    if (!isVisible())
    {
        return;
    }

    kmGLPushMatrix();

    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
        this->transformAncestors();
    }

    this->transform();
    this->beforeDraw();

    if(m_pChildren)
    {
        ccArray *arrayData = m_pChildren->data;
        unsigned int i=0;

        // draw children zOrder < 0
        for( ; i < arrayData->num; i++ )
        {
            CCNode *child =  (CCNode*)arrayData->arr[i];
            if ( child->getZOrder() < 0 )
            {
                child->visit();
            }
            else
            {
                break;
            }
        }

        // this draw
        this->draw();

        // draw children zOrder >= 0
        for( ; i < arrayData->num; i++ )
        {
            CCNode* child = (CCNode*)arrayData->arr[i];
            child->visit();
        }

    }
    else
    {
        this->draw();
    }

    this->afterDraw();
    if ( m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->afterDraw(this);
    }

    kmGLPopMatrix();// quick return if not visible
    if (!isVisible())
    {
        return;
    }

    kmGLPushMatrix();

    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
        this->transformAncestors();
    }

    this->transform();
    this->beforeDraw();

    if(m_pChildren)
    {
        ccArray *arrayData = m_pChildren->data;
        unsigned int i=0;

        // draw children zOrder < 0
        for( ; i < arrayData->num; i++ )
        {
            CCNode *child =  (CCNode*)arrayData->arr[i];
            if ( child->getZOrder() < 0 )
            {
                child->visit();
            }
            else
            {
                break;
            }
        }

        // this draw
        this->draw();

        // draw children zOrder >= 0
        for( ; i < arrayData->num; i++ )
        {
            CCNode* child = (CCNode*)arrayData->arr[i];
            child->visit();
        }

    }
    else
    {
        this->draw();
    }

    this->afterDraw();
    if ( m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->afterDraw(this);
    }

    kmGLPopMatrix();
}

cocos2d::CCSprite* ObserveLayer::getLastCell()
{
    return this->m_container->getLastCell();
}

cocos2d::CCArray* ObserveLayer::getCells()
{
    return m_container->getChildren();
}

//////////////////////////////////////////////////////////////////////////
///Clip EndRegion
//////////////////////////////////////////////////////////////////////////