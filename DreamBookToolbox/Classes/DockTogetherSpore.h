#ifndef __DockTogether_Spore_H__
#define __DockTogether_Spore_H__

#include "cocos2d.h"
#include "document.h"

USING_NS_CC;
using namespace std;
using namespace rapidjson;

class DockTogetherSpore : public CCScene
{
public:
	bool Do(CCNode* thisNode, CCArray *list, string jsonParam);
};

#endif
