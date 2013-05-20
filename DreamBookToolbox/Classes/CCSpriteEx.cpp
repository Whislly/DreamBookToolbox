#include "CCSpriteEx.h"

NS_CC_BEGIN

void CCSpriteEx::onEnter()
{
    CCSprite::onEnter();
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

    if (m_excuteLongClick)
    {
        if (m_isInTouch && !m_isInMove && m_longClickListener && m_longClickSelector && m_longClickEnabled)
        {
            m_originScale = getScale();
            this->setScale(m_originScale * 1.2f);
            this->setOpacity(200);
            m_afterLongPress = true;
            m_needRestoreAfterLongClick = true;

            (m_longClickListener->*m_longClickSelector)(this);
        }
    }
    else
    {
        m_needRestoreAfterLongClick = false;
    }
}

bool CCSpriteEx::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    if (this->isTouchInside(pTouch)) 
    {
        m_isInTouch = true;
        m_excuteLongClick = true;
        if ( m_doubleClickEnabled && isDoubleTouch()) 
        {
            if (m_doubleClickListener && m_doubleClickSelector)
            {
                (m_doubleClickListener->*m_doubleClickSelector)(this);
                m_isInTouch = false;
            }
        }
        else
        {
            if (m_singleClickListener && m_singleClickSelector && m_singleClickEnabled)
            {
                (m_singleClickListener->*m_singleClickSelector)(this);
            }
            if(m_longClickListener && m_longClickSelector && m_longClickEnabled)
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
    m_excuteLongClick = false;
    CCPoint deltaPoint = pTouch->getDelta();

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
    m_excuteLongClick = false;
    if(m_needRestoreAfterLongClick)
    {
        restoreStatus();
        m_needRestoreAfterLongClick = false;
    }
}

void CCSpriteEx::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
    if(m_excuteLongClick)
    {
        restoreStatus();
        m_excuteLongClick = false;
    }
}

void CCSpriteEx::setSelectorForSingleClick( CCObject *target, SEL_MenuHandler singleClickSelector )
{
    m_singleClickListener = target;
    m_singleClickSelector = singleClickSelector;
    m_singleClickEnabled = ((target != NULL) && (singleClickSelector != NULL));
}

void CCSpriteEx::setSelectorForDoubleClick( CCObject *target, SEL_MenuHandler doubleClickSelector )
{
    m_doubleClickListener = target;
    m_doubleClickSelector = doubleClickSelector;
    m_doubleClickEnabled = ((target != NULL) && (doubleClickSelector != NULL));
}

void CCSpriteEx::setSelectorForLongClick( CCObject *target, SEL_MenuHandler longClickSelector )
{
    m_longClickListener = target;
    m_longClickSelector = longClickSelector;
    m_longClickEnabled = ((target != NULL) && (longClickSelector != NULL));
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

CCSpriteEx::CCSpriteEx()
    : m_lastTouchTime(0)
    , m_isInTouch(false)
    , m_isInMove(false)
    , m_afterLongPress(false)
    , m_originScale(1.0f)
    , m_singleClickEnabled(false)
    , m_doubleClickEnabled(false)
    , m_longClickEnabled(false)
    , m_longClickListener(NULL)
    , m_longClickSelector(NULL)
    , m_singleClickListener(NULL)
    , m_singleClickSelector(NULL)
    , m_doubleClickListener(NULL)
    , m_doubleClickSelector(NULL)
    , m_excuteLongClick(false)
    , m_needRestoreAfterLongClick(false)
{

}

NS_CC_END