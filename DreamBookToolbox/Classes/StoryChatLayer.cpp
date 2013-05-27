#include "StoryChatLayer.h"

USING_NS_CC;

void StoryChatLayer::chat( CCNode* pSender )
{
    m_kongrong->stopAllActions();
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame *frame = cache->spriteFrameByName("5.png");
    m_kongrong->setDisplayFrame(frame);
    //m_kongrong->setScale(0.8f);

    m_olderBrother = CCSprite::createWithSpriteFrameName("7.png");
    m_youngBrother = CCSprite::createWithSpriteFrameName("6.png");

    addChild(m_olderBrother);
    m_olderBrother->setPosition(ccp(-100.0f, -70.0f));

    addChild(m_youngBrother);
    m_youngBrother->setScale(0.4f);
    m_youngBrother->setPosition(ccp(84.0f, -94.0f));

    m_olderBrother->runAction(CCFadeIn::create(1.0f));
    m_youngBrother->runAction(CCFadeIn::create(1.0f));

    m_feeling = CCSprite::createWithSpriteFrameName("Q1.png");
    CCArray* moreFrames = CCArray::createWithCapacity(5);
    char str[15];
    for(int i = 1; i < 6; i++) 
    {
        sprintf(str, "Q%d.png",i);
        CCSpriteFrame *frame = cache->spriteFrameByName(str);
        moreFrames->addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(moreFrames, 0.4f);
    animation->setLoops(1);
    addChild(m_feeling);
    m_feeling->setPosition(ccp(13, -50));
    m_feeling->runAction(CCAnimate::create(animation));
    m_feeling->runAction(CCRepeatForever::create( CCSequence::create( CCRotateBy::create(0.3f, -15), 
                         CCRotateBy::create(0.6f, 30), CCRotateBy::create(0.3f, -15), NULL)));

    m_caption = CCLabelTTF::create("Kong Rong has 3 pears, \nhe will give 2 pears to brothers.\n Which is his pear? \nplease tap the correct pear.", "Marker Felt", 26);
    m_caption->setColor(ccBLACK);
    addChild(m_caption);
    m_caption->setPosition(ccp(0, 65));

    //CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    m_bigPear = CCSprite::createWithSpriteFrameName("pear.png");
    m_bigPear->setScale(0.2f);
    addChild(m_bigPear);
    m_bigPear->setPosition(ccp(230, 45));

    m_middlePear = CCSprite::createWithSpriteFrameName("pear.png");
    m_middlePear->setScale(0.15f);
    addChild(m_middlePear);
    m_middlePear->setPosition(ccp(230, 5));

    m_smallPear = CCSprite::createWithSpriteFrameName("pear.png");
    m_smallPear->setScale(0.1f);
    addChild(m_smallPear);
    m_smallPear->setPosition(ccp(230, -35));
}

bool StoryChatLayer::init()
{
    CCSprite* pBkg = CCSprite::createWithSpriteFrameName("bkg.png");
    addChild(pBkg);

    pBkg = CCSprite::createWithSpriteFrameName("BigBackground.png");
    addChild(pBkg);

    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    m_kongrong = CCSprite::createWithSpriteFrameName("1.png");
    CCArray* moreFrames = CCArray::createWithCapacity(4);
    char str[15];
    for(int i = 1; i < 5; i++) 
    {
        sprintf(str, "%d.png",i);
        CCSpriteFrame *frame = cache->spriteFrameByName(str);
        moreFrames->addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(moreFrames, 0.4f);
    animation->setLoops(4);
    addChild(m_kongrong);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    m_kongrong->setPosition(ccp(-200.0f, -80.0f));
    m_kongrong->runAction(CCAnimate::create(animation));
    m_kongrong->runAction(CCSequence::create(CCMoveBy::create(1.6f, ccp(200.0f, 0)), 
                          CCCallFuncN::create(this, callfuncN_selector(StoryChatLayer::chat)), NULL));
    m_kongrong->setScale(0.8f);

    for (int i = 0; i < 3; i++)
    {
        m_correctBrothers[i] = false;
    }
    return true;
}

void StoryChatLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1/*priority=??*/, true);
}

void StoryChatLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

void StoryChatLayer::findPear( cocos2d::CCPoint localPos, cocos2d::CCSprite* pear )
{
    m_movingPear = NULL;
    if (!pear)
    {
        return;
    }
    CCRect rect = pear->boundingBox();
    bool isTouched = rect.containsPoint(localPos);
    if (isTouched)
    {
        m_movingPear = pear;
    }
}

bool StoryChatLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    CCPoint touchLocation = pTouch->getLocation(); // 返回GL坐标
    CCPoint localPos = convertToNodeSpace(touchLocation);
    CCArray* pArray = CCArray::create(m_middlePear, m_smallPear, m_bigPear, NULL);
    CCObject* pObj = NULL;
    int idx = 0;
    CCARRAY_FOREACH(pArray, pObj)
    {
        findPear(localPos, (CCSprite*)pObj);
        if (m_movingPear)
        {
            m_correctBrothers[idx] = false;
            break;
        }
        idx++;
    }
    return (m_movingPear != NULL);
}

void StoryChatLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
    CCPoint touchLocation = pTouch->getLocation(); // 返回GL坐标
    CCPoint localPos = convertToNodeSpace(touchLocation);
    m_movingPear->setPosition(localPos);
}

bool StoryChatLayer::hasGotPear( cocos2d::CCPoint localPos, cocos2d::CCSprite* brother )
{
    bool hasGot = false;
    if (brother)
    {
        CCRect rect = brother->boundingBox();
        hasGot = rect.containsPoint(localPos);
    }
    return hasGot;
}

void StoryChatLayer::smile( cocos2d::CCSprite* brother )
{
    m_feeling->stopAllActions();
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCArray* moreFrames = CCArray::createWithCapacity(5);
    char str[15];
    for(int i = 1; i < 6; i++) 
    {
        sprintf(str, "smile%d.png",i);
        CCSpriteFrame *frame = cache->spriteFrameByName(str);
        moreFrames->addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(moreFrames, 0.4f);
    animation->setLoops(1);
    //m_feeling->setPosition(ccpAdd(ccp(213, 20), brother->getPosition()));
    m_feeling->runAction(CCAnimate::create(animation));
    m_feeling->runAction(CCRepeatForever::create( CCSequence::create( CCRotateBy::create(0.3f, -15), 
        CCRotateBy::create(0.6f, 30), CCRotateBy::create(0.3f, -15), NULL)));
}

void StoryChatLayer::resetPosition(unsigned int i)
{
    CCPointArray* pPointArray = CCPointArray::create(3);
    pPointArray->addControlPoint(ccp(230, 5));
    pPointArray->addControlPoint(ccp(230, -35));
    pPointArray->addControlPoint(ccp(230, 45));

    ccBezierConfig config;
    config.controlPoint_1 = ccp(CCRANDOM_0_1() * 73.0f, CCRANDOM_0_1() * 20.0f);
    config.controlPoint_2 = ccp(CCRANDOM_0_1() * 146.0f, CCRANDOM_0_1() * -20.0f);
    config.endPosition = pPointArray->getControlPointAtIndex(i);

    m_movingPear->runAction(CCBezierTo::create(1.0f, config));
}

void StoryChatLayer::surprise( cocos2d::CCSprite* brother )
{
    m_feeling->stopAllActions();
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCArray* moreFrames = CCArray::createWithCapacity(5);
    char str[15];
    for(int i = 1; i < 6; i++) 
    {
        sprintf(str, "surprise%d.png",i);
        CCSpriteFrame *frame = cache->spriteFrameByName(str);
        moreFrames->addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(moreFrames, 0.4f);
    animation->setLoops(1);
    //m_feeling->setPosition(ccpAdd(ccp(213, 20), brother->getPosition()));
    m_feeling->runAction(CCAnimate::create(animation));
    m_feeling->runAction(CCRepeatForever::create( CCSequence::create( CCRotateBy::create(0.3f, -15), 
        CCRotateBy::create(0.6f, 30), CCRotateBy::create(0.3f, -15), NULL)));
}

void StoryChatLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
    CCPoint touchLocation = pTouch->getLocation(); // 返回GL坐标
    CCPoint localPos = convertToNodeSpace(touchLocation);
    CCArray* pArray = CCArray::create(m_olderBrother, m_kongrong, m_youngBrother, NULL);
    CCArray* pPearArray = CCArray::create(m_middlePear, m_smallPear, m_bigPear, NULL);
    CCObject* pObj = NULL;
    int i = 0;
    CCARRAY_FOREACH(pArray, pObj)
    {
        CCSprite* brother = (CCSprite*)pObj;
        if (hasGotPear(localPos, brother))
        {
            CCSprite* correctPear = (CCSprite*)pPearArray->objectAtIndex(i);
            if (correctPear == m_movingPear)
            {
                smile(brother);
                m_correctBrothers[i] = true;
            }
            else
            {
                surprise(brother);
                resetPosition(pPearArray->indexOfObject(m_movingPear));
            }
            break;
        }
        i++;
    }
    int correctCount = 0;
    for (i = 0; i < 3; i++)
    {
        if (m_correctBrothers[i])
        {
            correctCount++;
        }
    }
    if (correctCount == 3)
    {
        m_caption->setString("Modesty is a virtue in China.");
    }
    else
    {
        m_caption->setString("Kong Rong has 3 pears, \nhe will give 2 pears to brothers.\n Which is his pear? \nplease tap the correct pear.");
    }
    m_movingPear = NULL;
}

StoryChatLayer::StoryChatLayer()
    : m_movingPear(NULL)
    , m_bigPear(NULL)
    , m_middlePear(NULL)
    , m_smallPear(NULL)
    , m_olderBrother(NULL)
    , m_kongrong(NULL)
    , m_youngBrother(NULL)
    , m_caption(NULL)
    , m_feeling(NULL)
{

}

StoryChatLayer::~StoryChatLayer()
{
}
