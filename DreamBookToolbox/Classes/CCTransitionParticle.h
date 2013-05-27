#ifndef __CCTRANSITION_Particle_H__
#define __CCTRANSITION_Particle_H__

#include "cocos2d.h"

NS_CC_BEGIN
class CCTransitionParticle : public CCTransitionScene
{
protected:
    CCParticleSystemQuad* m_particle;
public:
    CCTransitionParticle();
    virtual ~CCTransitionParticle();
    virtual void onEnter();
    virtual void onExit();

    static CCTransitionParticle* create(float t, CCScene* scene, CCParticleSystemQuad* pParticle);
};
NS_CC_END
#endif // __CCTRANSITION_H__