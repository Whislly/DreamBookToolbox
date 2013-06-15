#ifndef __DockMoveSpore_Spore_H__
#define __DockMoveSpore_Spore_H__

#include "cocos2d.h"
#include "document.h"

USING_NS_CC;
using namespace std;
using namespace rapidjson;

class DockMoveSpore : public CCObject
{
public:
	bool Do(CCNode* thisNode, CCArray *list, string jsonParam);

	void Callback(CCNode* sender, void* data);
};

#endif
