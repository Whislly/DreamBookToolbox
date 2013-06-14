#include "DesignLayer.h"
#include "DBActionSprite.h"
#include "CommonHelper.h"
#include "document.h"
#include "filestream.h"
#include "prettywriter.h"

USING_NS_CC;

void DesignLayer::addTimeLabel()
{
    CCSize contentSize = this->getContentSize();
    m_lblTime = CCLabelBMFont::create("Time: 0", "font09.fnt");
    m_lblTime->setAnchorPoint(ccp(1, 1));
    this->addChild(m_lblTime, -1);
    m_lblTime->setPosition(ccp(contentSize.width * 0.5f - 20.0f, contentSize.height - 2.0f));
    m_lblTime->retain();
}

DesignLayer* DesignLayer::create( const cocos2d::ccColor4B& color )
{
    DesignLayer* pLayer = new DesignLayer();
    pLayer->initWithColor(color);
    pLayer->autorelease();
    return pLayer;
}

DesignLayer::DesignLayer()
    : m_lblTime(NULL)
    , m_time(0.0f)
    , m_currentSpriteTag(-1)
    , m_finishedActionCount(1)
    , m_enlarge(NULL)
    , m_reduce(NULL)
    , m_currentSprite(NULL)
    , m_input(NULL)
{
    m_enlarge = CCSpriteEx::create("enhance.png");
    addChild(m_enlarge, 5);
    m_enlarge->setScale(0.3f);
    m_enlarge->setSelectorForSingleClick(this, menu_selector(DesignLayer::enlarge));
    m_enlarge->setVisible(false);
    //m_enlarge->setPosition(ccp(300, 300));

    m_reduce = CCSpriteEx::create("small.png");
    addChild(m_reduce, 5);
    m_reduce->setScale(0.3f);
    //m_reduce->setPosition(ccp(300, 300));
    m_reduce->setSelectorForSingleClick(this, menu_selector(DesignLayer::reduce));
    m_reduce->setVisible(false);

    m_input = CCTextFieldTTF::textFieldWithPlaceHolder("", "Thonburi", 36);
    this->addChild(m_input, 99);
    m_input->setVisible(false);
}

DesignLayer::~DesignLayer()
{
    if(m_lblTime)
    {
        m_lblTime->release();
    }
    m_lblTime = NULL;
}

void DesignLayer::addSpriteEx( cocos2d::CCSpriteEx* pSprite )
{
    pSprite->setSelectorForDoubleClick(this, menu_selector(DesignLayer::showScaleToolButtons));
    this->addChild(pSprite, 0, pSprite->getTag());
}

void DesignLayer::addSpriteEx( cocos2d::CCSpriteEx* pSprite, int zOrder )
{
    pSprite->setSelectorForDoubleClick(this, menu_selector(DesignLayer::showScaleToolButtons));
    this->addChild(pSprite, zOrder, pSprite->getTag());
}

void DesignLayer::removeSpriteEx( int tag, bool cleanup )
{
    this->removeChildByTag(tag, cleanup);
}

bool DesignLayer::isContainsChild( int tag )
{
    CCNode* pChild = this->getChildByTag(tag);
    return (pChild != NULL);
}

void DesignLayer::stopChildrenActions( int tag )
{
    for (int i = tag; i >= 0; i--)
    {
        DBActionSprite* pActionSprite = (DBActionSprite*)(this->getChildByTag(i));
        if (pActionSprite)
        {
            pActionSprite->setEnableTime(false);
        }
    }
}

void DesignLayer::focusChild( cocos2d::CCSprite* pSprite )
{
    float scaleValue = pSprite->getScale();
    CCActionInterval* pAction = CCRepeatForever::create(
        (CCActionInterval*)CCSequence::create(CCScaleTo::create(0.4f, scaleValue * 1.1f), 
        CCScaleTo::create(0.4f, scaleValue * 0.9f), NULL)
        );
    pSprite->runAction(pAction);
    pAction->setTag(FocusActionTag);
}

void DesignLayer::designSpriteStatus( int tag )
{
    setEnableTime(false);
    DBActionSprite* pActionSprite = NULL;
    if(m_currentSpriteTag != -1)
    {
        pActionSprite = (DBActionSprite*)(this->getChildByTag(m_currentSpriteTag));
        if (pActionSprite)
        {
            //pActionSprite->setScaleEnabled(false);
        }
    }
    m_currentSpriteTag = tag;

    this->stopChildrenActions(tag);
    pActionSprite = (DBActionSprite*)(this->getChildByTag(tag));
    pActionSprite->designStatus(m_time);
    //pActionSprite->setScaleEnabled(true);
    this->focusChild(pActionSprite);
}   

void DesignLayer::runChildrenActions( int tag )
{
    for (int i = tag - 1; i >= 0; i--)
    {
        DBActionSprite* pActionSprite = (DBActionSprite*)(this->getChildByTag(i));
        if (pActionSprite)
        {
            pActionSprite->excuteAction(this->m_time);
        }
    }
}

void DesignLayer::saveData()
{
	std::string fileName = CCFileUtils::sharedFileUtils()->getWritablePath() + "data.json";
	FILE* file = fopen(fileName.c_str(), "w");
	rapidjson::FileStream stream(file);

	rapidjson::PrettyWriter<rapidjson::FileStream> writer(stream);
    
	writer.StartArray();
	CCObject* pObj = NULL;
    CCArray* pChildren = getChildren();
    CCARRAY_FOREACH(pChildren, pObj)
    {
        DBActionSprite* pActionSprite = (DBActionSprite*)(pObj);
        if (pActionSprite && (pActionSprite->getTag() != -1))
        {
            pActionSprite->save(writer);
        }
    }
	writer.EndArray();

	fclose(file);
}

void DesignLayer::loadData()
{
	std::string fileName = CCFileUtils::sharedFileUtils()->getWritablePath() + "data.json";
	FILE* file = fopen(fileName.c_str(), "r");
    if (!file)
    {
        return;
    }
	rapidjson::FileStream stream(file);

	rapidjson::Document doc;
	doc.ParseStream<0>(stream);

	if (doc.IsArray())
	{
		for (unsigned int i = 0; i < doc.Size(); ++i)
		{
			rapidjson::Value& value = doc[i];
			DBActionSprite* pActionSprite = DBActionSprite::create();
			pActionSprite->setTag(value["Tag"].GetInt());
			pActionSprite->load(value);
			addSpriteEx(pActionSprite);
			pActionSprite->readStatus(0.0f);
		}
	}

	fclose(file);
}

void DesignLayer::addFinishedActionCount()
{
    this->m_finishedActionCount++;
}

void DesignLayer::runChildrenActions()
{
    DBActionSprite* pActionSprite = NULL;
    if(m_currentSpriteTag != -1)
    {
        pActionSprite = (DBActionSprite*)(this->getChildByTag(m_currentSpriteTag));
        if (pActionSprite)
        {
            //pActionSprite->setScaleEnabled(false);
        }
    }
    m_currentSpriteTag = -1;
    m_lblTime->setString("Time: 0");
    m_time = 0.0f;
    setEnableTime(true);

    for (unsigned int i = 0; i < getChildrenCount(); i++)
    {
        DBActionSprite* pActionSprite = (DBActionSprite*)(this->getChildByTag(i));
        if (pActionSprite)
        {
            pActionSprite->excuteAction(this->m_time, this, callfunc_selector(DesignLayer::addFinishedActionCount));
        }
    }
}

void DesignLayer::activeActions( int tag )
{
    DBActionSprite* pActionSprite = NULL;
    if(m_currentSpriteTag != -1)
    {
        pActionSprite = (DBActionSprite*)(this->getChildByTag(m_currentSpriteTag));
        if (pActionSprite)
        {
            //pActionSprite->setScaleEnabled(false);
        }
    }
    m_currentSpriteTag = tag;
    m_lblTime->setString("Time: 0");
    m_time = 0.0f;

    pActionSprite = (DBActionSprite*)(this->getChildByTag(tag));
    if (pActionSprite)
    {
        /*m_time = pActionSprite->getStartTime();
        char time_str[20];
        sprintf(time_str, "Time: %.1f", this->m_time);
        m_lblTime->setString(time_str);*/
        pActionSprite->clearAction(m_time);
        pActionSprite->designAction(this->m_time);
        this->focusChild(pActionSprite);
        //pActionSprite->setScaleEnabled(true);
    }
    this->runChildrenActions(tag);
    setEnableTime(true);
}

void DesignLayer::step( float time )
{
    int temp = (int)(time * 10);
    this->m_time += (temp * 0.1);

    /*if (this->m_finishedActionCount == getChildrenCount())
    {
    setEnableTime(false);
    m_finishedActionCount = 1;
    }*/

    CCLOG("DesignLayer::step(%f)", m_time);

    char time_str[20];
    sprintf(time_str, "Time: %.1f", this->m_time);
    m_lblTime->setString(time_str);
}

void DesignLayer::setEnableTime( bool isTime )
{
    if (isTime)
    {
        this->schedule(schedule_selector(DesignLayer::step), 0.1f);
    }
    else
    {
        this->unschedule(schedule_selector(DesignLayer::step));
    }
}

void DesignLayer::enlarge( cocos2d::CCObject* pSender )
{
    if (!m_currentSprite)
    {
        return;
    }
    float scale = m_currentSprite->getScale();
    m_currentSprite->setScale(scale * 1.1f);
}

void DesignLayer::reduce( cocos2d::CCObject* pSender )
{
    if (!m_currentSprite)
    {
        return;
    }
    float scale = m_currentSprite->getScale();
    m_currentSprite->setScale(scale * 0.9f);
}

void DesignLayer::showScaleToolButtons( cocos2d::CCObject* pSender )
{
    DBActionSprite* pSprite = (DBActionSprite*)pSender;
    CCPoint pos = pSprite->getPosition();
    CCSize size = pSprite->getContentSize();
    m_enlarge->setVisible(true);
    m_reduce->setVisible(true);
    //m_input->setVisible(true);

    m_enlarge->setPosition(ccpAdd(pos, ccp(40.0f, 0.0f)));
    m_enlarge->runAction(CCShow::create());
    m_reduce->setPosition(ccpSub(pos, ccp(40.0f, 0.0f)));
    m_reduce->runAction(CCShow::create());

    m_input->setPosition(ccpAdd(pos, ccp(0.0f, size.height * 0.6f)));
    m_input->attachWithIME();
    m_input->runAction(CCShow::create());
    m_input->setString("");

    m_currentSprite = pSprite;
}

bool DesignLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    if(m_input->isVisible())
    {
        CCPoint touchLocation = pTouch->getLocation();
        if (m_enlarge->isVisible() && m_reduce->isVisible())
        {
            if (!m_reduce->boundingBox().containsPoint(touchLocation) && 
                !m_enlarge->boundingBox().containsPoint(touchLocation) &&
                !m_input->boundingBox().containsPoint(touchLocation) &&
                m_currentSprite && (!m_currentSprite->boundingBox().containsPoint(touchLocation)))
            {
                m_enlarge->setVisible(false);
                m_reduce->setVisible(false);
                m_input->setVisible(false);
                m_input->detachWithIME();
                const char* content = m_input->getString();
                if (content && strlen(content) > 0)
                {
                    m_currentSprite->setInputContent(content, m_time);
                    m_currentSprite->setSelectorForInput(this, menu_selector(DesignLayer::waitInput));
                }
            }
        }
        else
        {
            if (!m_input->boundingBox().containsPoint(touchLocation) &&
                m_currentSprite && (!m_currentSprite->boundingBox().containsPoint(touchLocation)))
            {
                m_input->setVisible(false);
                m_input->detachWithIME();
                #if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
                this->voiceRecognition(m_input->getString());
                #endif
            }
            else
            {

            }
        }
    }
    
    return false;
}

void DesignLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0/*priority=??*/, false);
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCDirector::sharedDirector()->setVoiceRecognitionTarget(this, voiceRecognition_selector(DesignLayer::voiceRecognition));
    #endif
}

void DesignLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCDirector::sharedDirector()->setVoiceRecognitionTarget(NULL, NULL);
    #endif
    CCLayerColor::onExit();
}

void DesignLayer::voiceRecognition( const char* voiceContent )
{
    if (m_currentSprite)
    {
        m_currentSprite->checkInputContent(voiceContent);
    }
}

void DesignLayer::waitInput( cocos2d::CCObject* pSender )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    this->activeVoiceService();
#else
    DBActionSprite* pSprite = (DBActionSprite*)pSender;
    CCPoint pos = pSprite->getPosition();
    CCSize size = pSprite->getContentSize();
    m_input->setVisible(true);
    m_input->runAction(CCShow::create());
    m_input->setPosition(ccpSub(pos, ccp(0.0f, size.height * 0.6f)));
    m_input->attachWithIME();
#endif
}
