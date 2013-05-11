#include "CCSpriteEx.h"

NS_CC_BEGIN

void CCSpriteEx::onEnter()
{
    CCSprite::onEnter();
    // 注册触摸协议，并吃掉触摸事件
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0/*priority=??*/, false);
}

void CCSpriteEx::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}

cocos2d::CCRect CCSpriteEx::rect()
{
    return CCRectMake(getPositionX() - getContentSize().width * getAnchorPoint().x,
        getPositionY() - getContentSize().height * getAnchorPoint().y,
        getContentSize().width, getContentSize().height); // 为了能够自定义自己的位置大小采用了低效的函数而不是直接用成员变量
}

bool CCSpriteEx::isTouchInside( CCTouch* touch )
{
    CCPoint touchLocation = touch->getLocation(); // 返回GL坐标
    CCPoint localPos = convertToNodeSpace(touchLocation);
    CCRect rc = rect();
    rc.origin = CCPointZero;
    bool isTouched = rc.containsPoint(localPos);

    return isTouched;
}

void CCSpriteEx::checkLongPress( float touchTime )
{
    this->unschedule(schedule_selector(CCSpriteEx::checkLongPress));

    if (m_isInTouch && !m_isInMove && m_pListener && m_longClickSelector)
    {
        this->setScale(m_originScale + 0.4f);
        this->setOpacity(200);
        m_afterLongPress = true;
        
        (m_pListener->*m_longClickSelector)(this);
    }
}

bool CCSpriteEx::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    m_originScale = getScale();
    if (this->isTouchInside(pTouch)) 
    {
        m_isInTouch = true;
        if (isDoubleTouch()) 
        {
            if (m_pListener && m_doubleClickSelector)
            {
                (m_pListener->*m_doubleClickSelector)(this);
            }
        }
        else
        {
            if (m_pListener && m_singleClickSelector)
            {
                (m_pListener->*m_singleClickSelector)(this);
            }
            if(m_pListener && m_longClickSelector)
            {
                this->schedule(schedule_selector(CCSpriteEx::checkLongPress), 1.0f);
            }
        }
        return true;
    }
    return false;
}

void CCSpriteEx::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
    CCPoint deltaPoint = pTouch->getDelta();

    CCLog("x = %f,y = %f", deltaPoint.x, deltaPoint.y);

    if(fabs(deltaPoint.x) > 1 || fabs(deltaPoint.y) > 1)
    {
        m_isInMove = true;
    }
}

void CCSpriteEx::restoreStatus()
{
    m_isInTouch = false;
    m_isInMove = false;
    m_afterLongPress = false;
    //恢复精灵
    this->setScale(m_originScale);
    //this->setPosition(m_orignalPoint);
    this->setOpacity(255);
}

void CCSpriteEx::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
    /*bool isTouched = isTouchInside(pTouch);
    if (isTouched)
    {      
    if (m_pListener && m_pfnSelector)
    {
    (m_pListener->*m_pfnSelector)(this);
    }
    }*/
    restoreStatus();
}

void CCSpriteEx::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
    restoreStatus();
}

void CCSpriteEx::setSelectorWithTarget( CCObject *target, SEL_MenuHandler singleClickSelector, SEL_MenuHandler doubleClickSelector, SEL_MenuHandler longClickSelector )
{
    m_pListener = target;
    m_singleClickSelector = singleClickSelector;
    m_doubleClickSelector = doubleClickSelector;
    m_longClickSelector = longClickSelector;
}

CCSpriteEx* CCSpriteEx::createWithSpriteFrameName( const char* pszFrameName )
{
    CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszFrameName);
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", pszFrameName);
    CCAssert(pFrame != NULL, msg);
#endif

    return createWithSpriteFrame(pFrame);
}


CCSpriteEx* CCSpriteEx::create( const char* pszFileName )
{
    CCSpriteEx* pSprite = CCSpriteEx::create();
    if (pSprite && pSprite->initWithFile(pszFileName))
    {
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

CCSpriteEx* CCSpriteEx::createWithSpriteFrame( CCSpriteFrame *pSpriteFrame )
{
    CCSpriteEx *pobSprite = CCSpriteEx::create();
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CCSpriteEx* CCSpriteEx::createWithTexture( cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect )
{
    CCSpriteEx *pobSprite = CCSpriteEx::create();
    if (pobSprite && pobSprite->initWithTexture(pTexture, rect))
    {
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

CCObject* CCSpriteEx::copy()
{
    CCSpriteEx* pSprite = CCSpriteEx::create();
    if (pSprite && pSprite->initWithTexture(CCSprite::getTexture(), CCSprite::getTextureRect()))
    {
        pSprite->setTag(this->getTag());
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}
NS_CC_END