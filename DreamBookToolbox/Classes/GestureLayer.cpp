#include "GestureLayer.h"
#include "SampleGestures.h"

USING_NS_CC;
using namespace DollarRecognizer;

void GestureLayer::init( cocos2d::CCParticleSystem* particle )
{
	m_target = NULL;
	m_selector = NULL;
	m_recognizer = new GeometricRecognizer();
	m_recognizer->loadTemplates();
	m_emitter = particle;
	addChild(particle);
	m_emitter->retain();
}

void GestureLayer::onEnter()
{
    CCLayer::onEnter();
    setTouchEnabled(true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 99, false);
    if(m_emitter)
    {
        m_emitter->setPosition(ccp(-100, -100));
    }
}

void GestureLayer::onExit()
{
    CCLayer::onExit();
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    if(m_emitter)
    {
        m_emitter->setPosition(ccp(-100, -100));
    }
}

GestureLayer* GestureLayer::create( cocos2d::CCParticleSystem* particle )
{
	GestureLayer* pGestureLayer = new GestureLayer();
	pGestureLayer->init(particle);
	return pGestureLayer;
}

GestureLayer::~GestureLayer()
{
    this->m_target = NULL;
    this->m_selector = NULL;
    if(m_emitter)
    {
        m_emitter->release();
    }
    m_emitter = NULL;
}

void GestureLayer::setGestureHandler( cocos2d::CCNode* pTarget, SEL_GestureHandler selector )
{
    this->m_target = pTarget;
    this->m_selector = selector;
}

bool GestureLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if (!m_target)
	{
		return false;
	}
    CCPoint location = pTouch->getLocationInView();
    Point2D p_Point2DTemp;
    p_Point2DTemp.x = location.x;
    p_Point2DTemp.y = location.y;
    m_2dPath.push_back(p_Point2DTemp);
    return true;
}

void GestureLayer::moveParticle( cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent )
{
	if (m_emitter != NULL)
	{
		CCPoint location = pTouch->getLocation();
		m_emitter->setPosition( location );
	}
}

void GestureLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
    CCPoint location = pTouch->getLocationInView();
    Point2D p_Point2DTemp;
    p_Point2DTemp.x = location.x;
    p_Point2DTemp.y = location.y;
    m_2dPath.push_back(p_Point2DTemp);
	moveParticle(pTouch, pEvent);
}

void GestureLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
    if (m_2dPath.size() < 1){
        return ;
    }
	RecognitionResult result = m_recognizer->recognize(m_2dPath);
	m_2dPath.clear();
	m_emitter->setPosition(ccp(-100, -100));
	(m_target->*m_selector)(result.name.c_str(), result.score);
}