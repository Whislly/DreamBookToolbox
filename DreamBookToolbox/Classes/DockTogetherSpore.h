#ifndef __DockTogetherSpore_Spore_H__
#define __DockTogetherSpore_Spore_H__

#include "cocos2d.h"
#include "document.h"

USING_NS_CC;
using namespace std;
using namespace rapidjson;

class DockTogetherSpore : public CCObject
{
public:
	bool Do(CCNode* thisNode, CCArray *list, string jsonParam);
};

#endif
