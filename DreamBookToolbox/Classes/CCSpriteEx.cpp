#include "CCSpriteEx.h"

NS_CC_BEGIN

void CCSpriteEx::onEnter()
{
    CCSprite::onEnter();
    // 注册触摸协议，并吃掉触摸事件
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0/*priority=??*/, false);

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    CCDirector::sharedDirector()->getOpenGLView()->SetWin32KeyLayer(this);
    #else
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
    #endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
}

void CCSpriteEx::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    CCDirector::sharedDirector()->getOpenGLView()->SetWin32KeyLayer(NULL);
    #endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
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

    if (m_isInTouch && !m_isInMove && m_longClickListener && m_longClickSelector && m_longClickEnabled)
    {
        this->setScale(m_originScale + 0.4f);
        this->setOpacity(200);
        m_afterLongPress = true;
        
        (m_longClickListener->*m_longClickSelector)(this);
    }
}

float CCSpriteEx::calcDistance( cocos2d::CCTouch *pTouch )
{
    /*CCRect rc = rect();
    CCPoint originPoint = rc.origin;*/
    //originPoint = ccp(originPoint.x + rc.size.width * 0.5f, originPoint.y + rc.size.height * 0.5f);
    CCPoint localPos = pTouch->getLocation(); // 返回GL坐标
    localPos = convertToNodeSpace(localPos);
    return (float)ccpDistance(CCPointZero, localPos);
}

float CCSpriteEx::calcDistance( CCSet *pTouches )
{
    CCSetIterator iter = pTouches->begin();
    CCTouch* pTouch = (CCTouch*)(*iter);
    CCPoint firstLocalPos = pTouch->getLocation(); // 返回GL坐标
    firstLocalPos = convertToNodeSpace(firstLocalPos);

    iter++;

    iter = pTouches->begin();
    pTouch = (CCTouch*)(*iter);
    CCPoint secondLocalPos = pTouch->getLocation(); // 返回GL坐标
    secondLocalPos = convertToNodeSpace(secondLocalPos);

    return ccpDistance(firstLocalPos, secondLocalPos);
}

void CCSpriteEx::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
    #if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    if(m_scaleEnabled && pTouches->count() == 2)
    {
        m_distance = calcDistance(pTouches);
    }
    #endif
}

void CCSpriteEx::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
    #if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    if(m_scaleEnabled && pTouches->count() == 2)
    {
        float newDistance = calcDistance(pTouches);
        float scale = newDistance / m_distance;
        //CCLog("newDistance(%f) / m_distance(%f) = %f", newDistance, m_distance, scale);
        setScale(scale);

        if( m_scaleListener && m_scaleSelector)
        {
            (m_scaleListener->*m_scaleSelector)(this);
        }
    }
    #endif
}

void CCSpriteEx::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
    #if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    if(m_scaleEnabled && pTouches->count() == 2)
    {
        float newDistance = calcDistance(pTouches);
        float scale = newDistance / m_distance;
        //CCLog("newDistance(%f) / m_distance(%f) = %f", newDistance, m_distance, scale);
        setScale(scale);

        if( m_scaleListener && m_scaleSelector)
        {
            (m_scaleListener->*m_scaleSelector)(this);
        }
    }
    #endif
}

void CCSpriteEx::ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent )
{

}

bool CCSpriteEx::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    m_originScale = getScale();
    if (this->isTouchInside(pTouch)) 
    {
        m_isInTouch = true;
        if (m_scaleEnabled)
        {
            //m_distance = calcDistance(pTouch);
            m_isInTouch = false;
        }
        else if ( m_doubleClickEnabled && isDoubleTouch()) 
        {
            if (m_doubleClickListener && m_doubleClickSelector)
            {
                (m_doubleClickListener->*m_doubleClickSelector)(this);
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
    if (m_scaleEnabled)
    {
        /*
        float newDistance = calcDistance(pTouch);
        float scale = newDistance / m_distance;
        //CCLog("newDistance(%f) / m_distance(%f) = %f", newDistance, m_distance, scale);
        setScale(scale);

        if( m_scaleListener && m_scaleSelector)
        {
            (m_scaleListener->*m_scaleSelector)(this);
        }
        */
    }
    else
    {
        CCPoint deltaPoint = pTouch->getDelta();

        if(fabs(deltaPoint.x) > 1 || fabs(deltaPoint.y) > 1)
        {
            m_isInMove = true;
        }
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
    if(m_scaleEnabled)
    {
        /*
        if( m_scaleListener && m_scaleSelector)
        {
            (m_scaleListener->*m_scaleSelector)(this);
        }
        */
    }
    else
    {
        restoreStatus();
    }
}

void CCSpriteEx::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
    restoreStatus();
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

void CCSpriteEx::setSelectorForScale( CCObject* target, SEL_MenuHandler scaleSelector )
{
    m_scaleListener = target;
    m_scaleSelector = scaleSelector;
    m_scaleEnabled = ((target != NULL) && (scaleSelector != NULL));
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void CCSpriteEx::processWin32KeyPress( UINT message, WPARAM wParam, LPARAM lParam )
{
    float scale = getScale();
    if (!m_scaleEnabled)
    {
        return;
    }
    switch(message)
    {
    case WM_MOUSEWHEEL:
        short Delta = (short)HIWORD(wParam);
        if (Delta > 0)
        {
            setScale(scale + 0.1f);
        }
        else
        {
            setScale(scale - 0.1f);
        }
        
        if( m_scaleListener && m_scaleSelector)
        {
            (m_scaleListener->*m_scaleSelector)(this);
        }
        break;
    }
}
#endif

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
    , m_scaleEnabled(false)
    , m_singleClickEnabled(false)
    , m_doubleClickEnabled(false)
    , m_longClickEnabled(false)
    , m_scaleListener(NULL)
    , m_scaleSelector(NULL)
    , m_longClickListener(NULL)
    , m_longClickSelector(NULL)
    , m_singleClickListener(NULL)
    , m_singleClickSelector(NULL)
    , m_doubleClickListener(NULL)
    , m_doubleClickSelector(NULL)
{

}

NS_CC_END