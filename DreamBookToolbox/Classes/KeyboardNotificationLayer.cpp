#include "KeyboardNotificationLayer.h"


KeyboardNotificationLayer::KeyboardNotificationLayer(void)
{
		this->current = NULL;
		this->nodeArray = CCArray::create();
		this->nodeArray->retain();
		this->setTouchEnabled(true);
}

KeyboardNotificationLayer::~KeyboardNotificationLayer(void)
{
		if (this->nodeArray != NULL)
		{
				this->nodeArray->release();
		}
}

void KeyboardNotificationLayer::registerWithTouchDispatcher()
{
		CCDirector* pDirector = CCDirector::sharedDirector();
		pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void KeyboardNotificationLayer::keyboardWillShow(CCIMEKeyboardNotificationInfo& info)
{
		/*
		if (this->current == NULL)
		{
				return;
		}

		CCRect rectTracked = CommonHelper::GetRect(this->current);
		// if the keyboard area doesn't intersect with the tracking node area, nothing need to do.
		if (! rectTracked.intersectsRect(info.end))
		{
				return;
		}

		// assume keyboard at the bottom of screen, calculate the vertical adjustment.
		float adjustVert = info.end.getMaxY() - rectTracked.getMinY();

		// move all the children node of KeyboardNotificationLayer
		CCArray * children = getChildren();
		CCNode * node = 0;
		int count = children->count();
		CCPoint pos;
		for (int i = 0; i < count; ++i)
		{
				node = (CCNode*)children->objectAtIndex(i);
				pos = node->getPosition();
				pos.y += adjustVert;
				node->setPosition(pos);
		}
		*/
}

// CCLayer function

bool KeyboardNotificationLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
		this->beginPos = pTouch->getLocation();    
		return true;
}

void KeyboardNotificationLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
		//if no node was watching, return, no handle anymore.
		int count = this->nodeArray->count();
		if (count == 0)
		{
				return;
		}

		// decide the trackNode is clicked.
		CCRect rect;
		CCPoint point = convertTouchToNodeSpaceAR(pTouch);

		//loop InputElement inside, decide to call which
		InputElement* preFocus = this->current;
		InputElement *nowFocus = NULL;
		for (int i = 0; i < count; i++)
		{
				InputElement *input = (InputElement*)this->nodeArray->objectAtIndex(i);
				CCRect rect = input->GetRangeRect();

				if (rect.containsPoint(point))
				{
						nowFocus = input;
						break;
				}
		}
		this->current = nowFocus;

		//if select on the same object, do nothing
		if (nowFocus == preFocus)
		{
				return;
		}

		//leave pre input
		if (preFocus != NULL)
		{
				preFocus->OnLeave();
		}

		//enter new select input
		if (nowFocus != NULL)
		{
				nowFocus->OnEnter();
		}
}
