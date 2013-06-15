#include "EFTestLayer.h"

EFTestLayer::EFTestLayer(void)
{
	this->offset = CCPointZero;
	this->currentSelect = NULL;
	this->elementArr = NULL;
	this->togetherSpore = NULL;
	this->moveSpore = NULL;
}

EFTestLayer::~EFTestLayer()
{
	if (this->elementArr != NULL)
	{
		this->elementArr->release();
	}

	if (this->togetherSpore != NULL)
	{
		delete this->togetherSpore;
	}

	if (this->moveSpore != NULL)
	{
		delete this->moveSpore;
	}
}

bool EFTestLayer::init()
{
	this->setTouchEnabled(true);
	this->togetherSpore = new DockTogetherSpore();
	this->moveSpore = new DockMoveSpore();

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	//add back menu
	CCMenuItemSprite *pBackItem = CCMenuItemSprite::create(
		CCSprite::createWithSpriteFrameName("b1.png"),
		CCSprite::createWithSpriteFrameName("b1.png"),
		this,
		menu_selector(EFTestLayer::menuBackCallback));
	pBackItem->setPosition(ccp(winSize.width - 50, 20));

	// Create a menu with the "close" menu item, it's an auto release object.
	CCMenu* pMenu = CCMenu::create(pBackItem, NULL);
	pMenu->setPosition(CCPointZero);
	// Add the menu to HelloWorld layer as a child layer.
	this->addChild(pMenu, 1);

	//init
	this->elementArr = CCArray::create();
	this->elementArr->retain();

	//add move action
	CCDictionary *leftDic = CCDictionary::create();
	leftDic->retain();
	leftDic->setObject(ccs("A"), "group");
	leftDic->setObject(ccs("1"), "index");

	CCDictionary *rightDic = CCDictionary::create();
	rightDic->retain();
	rightDic->setObject(ccs("B"), "group");
	rightDic->setObject(ccs("1"), "index");

	//
	CCSprite *left = CCSprite::create("Images/EFTest/left.png");
	left->setUserData(leftDic);
	left->setPosition(ccp(300, 200));
	this->addChild(left);
	this->elementArr->addObject(left);

	CCSprite *right = CCSprite::create("Images/EFTest/right.png");
	right->setUserData(rightDic);
	right->setPosition(ccp(500, 200));
	this->addChild(right);
	this->elementArr->addObject(right);

	CCArray *arr = CCArray::create();
	arr->addObject(right);

	return true;
}

void EFTestLayer::menuBackCallback( CCObject* pSender )
{
	CCDirector::sharedDirector()->replaceScene(MainLayer::scene());
}

//=============================== Touches ===============================
//Begin
void EFTestLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it;
	CCTouch* touch;

	this->currentSelect = NULL;

	for ( it = pTouches->begin(); it != pTouches->end(); it++)
	{
		touch = (CCTouch*)(*it);

		if(!touch)
			break;

		CCPoint location = touch->getLocation();

		for (int i = 0; i < this->elementArr->count(); i++)
		{
			CCSprite *s = (CCSprite*)this->elementArr->objectAtIndex(i);

			//only allow move group B
			CCDictionary *dic = (CCDictionary*)s->getUserData();
			CCString *str = (CCString*)dic->objectForKey("group");
			if (strcmp(str->getCString(), "A") == 0)
			{
				continue;
			}

			//CCRect rect = s->boundingBox();
			CCPoint thePos = s->getPosition();
			CCSize theSize = s->getContentSize();
			CCRect rect = CCRect(thePos.x - theSize.width / 2, thePos.y - theSize.height / 2, theSize.width, theSize.height);
			//CCRect rect = CCRect(s->getPosition().x, s->getPosition().y, s->getContentSize().width, s->getContentSize().height);
			//selected
			if (rect.containsPoint(location))
			{
				this->currentSelect = s;
				this->currentSelect->stopAllActions();

				this->offset.x = thePos.x - location.x;
				this->offset.y = thePos.y - location.y;

				break;
			}
		}
	}
}

void EFTestLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	if (this->currentSelect == NULL)
	{
		return;
	}

	CCSetIterator it;
	CCTouch* touch;
	for ( it = pTouches->begin(); it != pTouches->end(); it++)
	{
		touch = (CCTouch*)(*it);

		if(!touch)
			break;

		CCPoint location = touch->getLocation();
		location.x += this->offset.x;
		location.y += this->offset.y;
		this->currentSelect->setPosition(location);

		this->moveSpore->Do(this->currentSelect, this->elementArr, "");
	}
}

void EFTestLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	if (this->currentSelect == NULL)
	{
		return;
	}


	//if match, do the action
	this->togetherSpore->Do(this->currentSelect, this->elementArr, "ABC");
	/*
	//Add a new body/atlas sprite at the touched location
	CCSetIterator it;
	CCTouch* touch;

	for ( it = pTouches->begin(); it != pTouches->end(); it++)
	{
	touch = (CCTouch*)(*it);

	if(!touch)
	break;

	CCPoint location = touch->getLocation();

	CCSprite* sEnd = NULL;
	for (int i = 0; i < this->elementArr->count(); i++)
	{
	CCSprite *s = (CCSprite*)this->elementArr->objectAtIndex(i);
	//CCRect rect = s->boundingBox();

	CCPoint thePos = s->getPosition();
	CCSize theSize = s->getContentSize();
	CCRect rect = CCRect(thePos.x, thePos.y, theSize.width, theSize.height);
	//selected
	if (rect.containsPoint(location))
	{
	sEnd = s;
	break;
	}
	}

	//click (begin / end at the same object)
	if (sEnd == this->currentSelect)
	{
	//...
	}
	}
	*/

	this->currentSelect = NULL;
}

void EFTestLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
}

//End


