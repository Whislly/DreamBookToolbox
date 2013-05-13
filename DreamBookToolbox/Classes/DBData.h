#ifndef __DBData_H__
#define __DBData_H__

#include "cocos2d.h"
#include "DBPropertyData.h"

class DBData : public cocos2d::CCObject
{
protected:
    cocos2d::CCDictionary* m_dic;
    virtual bool init();
    void addDBPropertyData(float time, DBPropertyData* propertyData);
public:
    DBData();
    ~DBData();
    static DBData* create();
    DBPropertyData* getDBPropertyData(float time);
    void removeDBPropertyData(float startTime, float endTime);
    CC_SYNTHESIZE(float, m_startTime, StartTime);
    CC_SYNTHESIZE(float, m_endTime, EndTime);
};

#endif  // __DBData_H__