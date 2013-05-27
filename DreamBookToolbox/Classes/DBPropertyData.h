#ifndef __DBPropertyData_H__
#define __DBPropertyData_H__

#include "cocos2d.h"

class DBPropertyData : public cocos2d::CCObject
{
protected:
    cocos2d::CCPoint m_pos;
public:
    DBPropertyData();
    ~DBPropertyData();
    static DBPropertyData* create();
    cocos2d::CCPoint getPosition();    
    void setPosition(cocos2d::CCPoint& pos);
    CC_SYNTHESIZE(float, m_scale, Scale);
    void save(int tag, int time);
    void load(int tag, int time);
protected:
    CC_SYNTHESIZE_READONLY(cocos2d::CCString*, m_inputContent, InputContent);
    void setInputContent(const char* content);
};

#endif  // __DBPropertyData_H__