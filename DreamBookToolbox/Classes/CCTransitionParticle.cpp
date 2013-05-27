#include "CCTransitionParticle.h"

USING_NS_CC;

cocos2d::CCTransitionParticle::CCTransitionParticle()
    : m_particle(NULL)
{

}

cocos2d::CCTransitionParticle::~CCTransitionParticle()
{
    if (m_particle)
    {
        m_particle->release();
        m_particle = NULL;
    }
}

void cocos2d::CCTransitionParticle::onEnter()
{
    CCTransitionScene::onEnter();
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    m_pInScene->runAction(CCFadeIn::create(m_fDuration / 2));

    if(m_particle)
    {
        addChild(m_particle);
        m_particle->setPosition(CCPointZero);
        CCActionInterval* pAvtion = CCSequence::create(CCMoveBy::create(m_fDuration * 0.05f, ccp(s.width, 0)), CCMoveBy::create(m_fDuration * 0.1f, ccp(0 - s.width, s.height * 0.1f)), NULL);
        m_particle->runAction(CCSequence::create(CCRepeat::create(pAvtion, 10), 
            CCCallFunc::create(this, callfunc_selector(CCTransitionScene::hideOutShowIn)),
            CCCallFunc::create(this, callfunc_selector(CCTransitionScene::finish)), 
            NULL));
    }

    m_pOutScene->setVisible(false);
    m_pOutScene->runAction(CCSequence::create(CCDelayTime::create(m_fDuration * 0.5f), CCFadeOut::create(m_fDuration * 0.5f), NULL));
}

CCTransitionParticle* cocos2d::CCTransitionParticle::create( float t, CCScene* scene, CCParticleSystemQuad* pParticle )
{
    CCTransitionParticle* pScene = new CCTransitionParticle();
    if(pScene && pParticle && pScene->initWithDuration(t, scene))
    {
        pScene->m_particle = pParticle;
        pScene->m_particle->retain();
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void cocos2d::CCTransitionParticle::onExit()
{
    CCTransitionScene::onExit();
}