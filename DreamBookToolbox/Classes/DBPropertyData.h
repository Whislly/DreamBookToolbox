#ifndef __DBPropertyData_H__
#define __DBPropertyData_H__

#include "cocos2d.h"
#include "prettywriter.h"
#include "filestream.h"
#include "document.h"

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
    void save(rapidjson::PrettyWriter<rapidjson::FileStream>& write);
    void load(rapidjson::Value& data);
protected:
    cocos2d::CCString* m_inputContent;
public:
    const char* getInputContent();
    void setInputContent(const char* content);
};

#endif  // __DBPropertyData_H__