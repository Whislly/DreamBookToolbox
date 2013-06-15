#include "DockTogetherSpore.h"

bool DockTogetherSpore::Do(CCNode* thisNode, CCArray *list, string jsonParam)
{
	if (list == NULL)
	{
		return false;
	}

	for (int i = 0; i < list->count(); i++)
	{
		CCNode *dockNode = (CCNode*)list->objectAtIndex(i);
		if (dockNode == thisNode)
		{
			continue;
		}

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
			continue;
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

		CCMoveTo *moveTo = CCMoveTo::create(1.0F, ccp(point.x + xOffset, point.y + yOffset));
		thisNode->runAction(moveTo);
		//moveTo->create();

		return true;
	}

	return false;
}
