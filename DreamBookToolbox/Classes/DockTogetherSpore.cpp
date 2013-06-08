#include "DockTogetherSpore.h"

bool DockTogetherSpore::Do(CCNode* thisNode, CCArray *list, string jsonParam)
{
	if (list == NULL)
	{
		return false;
	}

	if (list->count() != 1)
	{
		return false;
	}

	CCNode *dockNode = (CCNode*)list->objectAtIndex(0);
	
	CCRect thisRect = thisNode->boundingBox();
	CCRect dockRect = dockNode->boundingBox();

	//minx   =   max(minx1,   minx2)  
    //miny   =   max(miny1,   miny2)  
    //maxx   =   min(maxx1,   maxx2)  
    //maxy   =   min(maxy1,   maxy2)  
	float minX = max(thisRect.getMinX(), dockRect.getMinX());
	float minY = max(thisRect.getMinY(), dockRect.getMinY());
	float maxX = min(thisRect.getMaxX(), dockRect.getMaxX());
	float maxY = min(thisRect.getMaxY(), dockRect.getMaxY());

	//£¨ minx  >  maxx £© »òÕß £¨ miny  >  maxy £© 
	if ((minX > maxX) || (minY > maxY))
	{
		return false;
	}


	//parse json
	
	//TODO here : rapidjson
	
	//Document doc;
	//Value groupA("A");
	//Value valueA(1);
	//doc.AddMember<0>("group", groupA);
	//doc.AddMember("value", valueA, doc.GetAllocator());

	//NOTICE : can move this to outside
	//running dock animation
	CCPoint point = dockNode->getPosition();

	float xOffset = dockRect.size.width;
	float yOffset = 0.0F;

	CCMoveTo *moveTo = CCMoveTo::create(2.0F, ccp(point.x + xOffset, point.y + yOffset));
	thisNode->runAction(moveTo);
	//moveTo->create();

	return true;
}
