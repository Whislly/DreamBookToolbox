#include "DockMoveSpore.h"

bool DockMoveSpore::Do(CCNode* thisNode, CCArray *list, string jsonParam)
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

		CCDictionary *data = (CCDictionary*)dockNode->getUserData();
		if (data->objectForKey("__Moving") != NULL)
		{
			continue;
		}
		
		int offsetDockX = 50;
		int offsetDockY = 20;

		//check if together
		CCRect thisRect = thisNode->boundingBox();
		CCRect dockRect = dockNode->boundingBox();
		float minX = max(thisRect.getMinX(), dockRect.getMinX());
		float minY = max(thisRect.getMinY(), dockRect.getMinY());
		float maxX = min(thisRect.getMaxX(), dockRect.getMaxX() + offsetDockX);
		float maxY = min(thisRect.getMaxY(), dockRect.getMaxY() + offsetDockY);
		//not docked
		if ((minX > maxX) || (minY > maxY))
		{
			if (data->objectForKey("__Moved") == NULL)
			{
				continue;
			}
			data->setObject(ccs("YES"), "__Moving");
			data->removeObjectForKey("__Moved");

			//Move Back
			CCPoint point = dockNode->getPosition();

			float xOffset = -50.0F;
			float yOffset = 0.0F;

			CCMoveTo *moveTo = CCMoveTo::create(1.0F, ccp(point.x + xOffset, point.y + yOffset));
			CCCallFuncND *callback = CCCallFuncND::create(this, callfuncND_selector(DockMoveSpore::Callback), dockNode);
			CCSequence *seq = CCSequence::create(moveTo, callback, NULL);
			dockNode->runAction(seq);
		}
		//docked
		else
		{
			if (data->objectForKey("__Moved") != NULL)
			{
				continue;
			}
			data->setObject(ccs("YES"), "__Moving");
			data->setObject(ccs("YES"), "__Moved");

			//Move To
			CCPoint point = dockNode->getPosition();

			float xOffset = 50.0F;
			float yOffset = 0.0F;

			CCMoveTo *moveTo = CCMoveTo::create(1.0F, ccp(point.x + xOffset, point.y + yOffset));
			CCCallFuncND *callback = CCCallFuncND::create(this, callfuncND_selector(DockMoveSpore::Callback), dockNode);
			CCSequence *seq = CCSequence::create(moveTo, callback, NULL);
			dockNode->runAction(seq);
		}
	}

	return false;
}

void DockMoveSpore::Callback(CCNode* sender, void* data)
{
	CCNode *dockNode = (CCNode*)data;
	CCDictionary *dic = (CCDictionary*)dockNode->getUserData();
	dic->removeObjectForKey("__Moving");
	return;
}