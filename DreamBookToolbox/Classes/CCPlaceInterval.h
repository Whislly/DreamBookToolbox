#ifndef __CCPlace_Interval_H__
#define __CCPlace_Interval_H__

#include "cocos2d.h"

NS_CC_BEGIN
class CCPlaceInterval : public CCActionInterval
{
protected:
    CCPointArray* m_pointArray;
    int m_idx;
public:
    static CCPlaceInterval* create(float duration, CCPointArray* pointArray);
    virtual void update( float time );
    virtual CCObject* copyWithZone( CCZone *pZone );
    CCPlaceInterval();
    ~CCPlaceInterval();
};
NS_CC_END
#endif // __CCPlace_Interval_H__