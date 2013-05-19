#include "PicPickupLayer.h"

PicPickupLayer::PicPickupLayer(void)
{
	this->selectionLayer = NULL;
	this->container = NULL;
}

PicPickupLayer::~PicPickupLayer(void)
{
}

void PicPickupLayer::update(float dt)
{
	//It is recommended that a fixed time step is used with Box2D for stability
	//of the simulation, however, we are using a variable time step here.
	//You need to make an informed choice, the following URL is useful
	//http://gafferongames.com/game-physics/fix-your-timestep/

	int velocityIterations = 8;
	int positionIterations = 1;

	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	//world->Step(dt, velocityIterations, positionIterations);
}

bool PicPickupLayer::init()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	//CCPoint leftTop = VisibleRect::leftTop();

	//
	cocos2d::CCLabelTTF *label = CCTextFieldTTF::textFieldWithPlaceHolder("Search :", CCSize(0, 0), kCCTextAlignmentLeft, "Thonburi", 32);
	label->setColor(ccc3(121, 112, 1));
	label->setPosition(ccp(100, winSize.height - 100));
	label->setAnchorPoint(CCPointZero);
	this->addChild(label);

	//
	CCSprite *search = CCSprite::create("Images/PicPickup/search.png");
	search->setPosition(ccp(250, winSize.height - 120));
	search->setAnchorPoint(CCPointZero);
	this->addChild(search);

	//
	this->container = DynamicLayerContainer::create(Pic_Count);
	this->container->setAnchorPoint(CCPointZero);
	this->container->setPosition(0, 0);
	this->addChild(this->container);

	//
	this->selectionLayer = PicSelectionLayer::create();
	selectionLayer->setAnchorPoint(CCPointZero);
	selectionLayer->setPosition(winSize.width - 240 - 10, 30);
	this->addChild(selectionLayer);

	CCMenuItemSprite *pBackItem = CCMenuItemSprite::create(
		CCSprite::createWithSpriteFrameName("b1.png"),
		CCSprite::createWithSpriteFrameName("b1.png"),
		this,
		menu_selector(PicPickupLayer::menuBackCallback));
	pBackItem->setPosition(ccp(winSize.width - 50, 20));

	CCMenuItemSprite *preItem = CCMenuItemSprite::create(
		CCSprite::createWithSpriteFrameName("b2.png"),
		CCSprite::createWithSpriteFrameName("b2.png"),
		this,
		menu_selector(PicPickupLayer::menuPreCallback));
	preItem->setAnchorPoint(CCPointZero);
	preItem->setPosition(ccp(30, 10));

	CCMenuItemSprite *subItem = CCMenuItemSprite::create(
		CCSprite::create("b3.png"),
		CCSprite::create("b3.png"),
		this,
		menu_selector(PicPickupLayer::menuSubCallback));
	int width = winSize.width - 30 - 260 + 30 - subItem->getContentSize().width;
	subItem->setAnchorPoint(CCPointZero);
	subItem->setPosition(ccp(width, 10));

	// Create a menu with the "close" menu item, it's an auto release object.
	CCMenu* pMenu = CCMenu::create(pBackItem, preItem, subItem, NULL);
	pMenu->setPosition(CCPointZero);

	// Add the menu to HelloWorld layer as a child layer.
	this->addChild(pMenu, 1);


	//init the show layer

	PicShowLayer *current = this->container->GetCurrentShowLayer();
	current->setPosition(30, 30);

	return true;
}

void PicPickupLayer::draw()
{
	//
	// IMPORTANT:
	// This is only for debug purposes
	// It is recommend to disable it
	//
	CCLayer::draw();

	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

	kmGLPushMatrix();

	kmGLPopMatrix();
}

void PicPickupLayer::ClickOnSprite(CCPoint location, CCSprite *sprite)
{
	float scale = sprite->getScale();

	//pos
	CCSize newSize = sprite->getContentSize();
	newSize = CCSize(newSize.width * 0.5, newSize.height * 0.5);
	CCPoint newPos = this->selectionLayer->GetPreAddPos(newSize);
	CCPoint selectLayerPos = this->selectionLayer->getPosition();
	newPos = ccp(newPos.x + selectLayerPos.x, newPos.y + selectLayerPos.y);

	//add new sprite to right panel
	CCSprite *newSprite = CommonHelper::CloneSprite(sprite);
	newSprite->setPosition(newPos);
	newSprite->setScale(0.5 * scale);
	newSprite->setContentSize(newSize);
	newSprite->setVisible(false);
	this->selectionLayer->AddElement(newSprite);

	//
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	//run the sport sprite move action
	CCSprite *sportSprite = CommonHelper::CloneSprite(sprite);
	CCPoint p = sprite->getPosition();
	sportSprite->setPosition(ccp(p.x + 30, p.y + 30));
	this->addChild(sportSprite);

	CCFiniteTimeAction *move = CCSequence::create(
		CCMoveTo::create(2, newPos),
		CCCallFuncND::create(this, callfuncND_selector(PicPickupLayer::RemoveSportSpriteEvent), (void*)sportSprite), 
		CCCallFuncND::create(this, callfuncND_selector(PicPickupLayer::TransferSpriteEvent), (void*)newSprite), 
		NULL);
	sportSprite->runAction(move);

	CCActionInterval*  actionBy = CCRotateBy::create(2,  360);
	sportSprite->runAction(actionBy);

	CCScaleTo *scaleAction = CCScaleTo::create(2, 0.5 * scale, 0.5 * scale);
	sportSprite->runAction(scaleAction);

	return;
}

//=============================== Touches ===============================
//Begin

void PicPickupLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it;
	CCTouch* touch;

	for ( it = pTouches->begin(); it != pTouches->end(); it++)
	{
		touch = (CCTouch*)(*it);

		if(!touch)
			break;

		CCPoint location = touch->getLocation();

	}
}

void PicPickupLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it;
	CCTouch* touch;

	for ( it = pTouches->begin(); it != pTouches->end(); it++)
	{
		touch = (CCTouch*)(*it);

		if(!touch)
			break;

		CCPoint location = touch->getLocation();
	}
}

void PicPickupLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	//Add a new body/atlas sprite at the touched location
	CCSetIterator it;
	CCTouch* touch;

	for ( it = pTouches->begin(); it != pTouches->end(); it++)
	{
		touch = (CCTouch*)(*it);

		if(!touch)
			break;

		CCPoint location = touch->getLocation();

	}
}

void PicPickupLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
}

//End

void PicPickupLayer::RemoveSportSpriteEvent(CCNode* sender, void* data)
{
	CCSprite *sprite = (CCSprite*)data;
	this->removeChild(sprite, true);
}


void PicPickupLayer::TransferSpriteEvent(CCNode* sender, void* data)
{
	CCSprite *sprite = (CCSprite*)data;
	sprite->setVisible(true);
}

CCArray* PicPickupLayer::getElementArray()
{
	CCArray* pElementArray = NULL;
	if (selectionLayer)
	{
		pElementArray = selectionLayer->getElementArray();
	}
	return pElementArray;
}

void PicPickupLayer::setSelectorWithTarget( CCObject *target, SEL_ImportElementsHandler importElementsSelector )
{
	this->m_pListener = target;
	this->m_importElementsSelector = importElementsSelector;
}

cocos2d::CCScene* PicPickupLayer::scene(CCObject *target, SEL_ImportElementsHandler importElementsSelector)
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		PicPickupLayer *layer = PicPickupLayer::create();
		CC_BREAK_IF(! layer);

		layer->setSelectorWithTarget(target, importElementsSelector);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

void PicPickupLayer::menuBackCallback( CCObject* pSender )
{
	if (this->m_pListener && this->m_importElementsSelector)
	{
		(m_pListener->*m_importElementsSelector)(this, selectionLayer->getElementArray());
	}

	CCDirector::sharedDirector()->popScene();
}

void PicPickupLayer::menuPreCallback( CCObject* pSender )
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	PicShowLayer *current = this->container->GetCurrentShowLayer();
	PicShowLayer *pre = this->container->GoToPre();
	if (pre == NULL)
	{
		return;
	}

	//move current
	current->stopAllActions();
	CCMoveTo *moveCur = CCMoveTo::create(2, ccp(winSize.width + 30, 30));
	current->runAction(moveCur);
	CCOrbitCamera *orbitCur = CCOrbitCamera::create(2, 1, 0, 0, 90, 0, 0);
	current->runAction(orbitCur);

	//move pre
	pre->stopAllActions();
	pre->setVisible(true);
	pre->setPosition(ccp(-winSize.width - 30, 30));
	//pre->setPosition(-winSize.width - 30, 30);
	//
	CCOrbitCamera *orbitPre = CCOrbitCamera::create(2, 1, 0, -90, 90, 0, 0);
	pre->runAction(orbitPre);
	CCMoveTo *movePre = CCMoveTo::create(2, ccp(30, 30));
	pre->runAction(movePre);
}

void PicPickupLayer::menuSubCallback( CCObject* pSender )
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	PicShowLayer *current = this->container->GetCurrentShowLayer();
	PicShowLayer *next = this->container->GoToNext();
	if (next == NULL)
	{
		return;
	}

	//move current
	current->setPosition(ccp(30, 30));
	current->stopAllActions();
	//
	CCMoveTo *moveCur = CCMoveTo::create(2, ccp(-winSize.width - 30, 30));
	current->runAction(moveCur);	
	CCOrbitCamera *orbitCur = CCOrbitCamera::create(2, 1, 0, 0, -90, 0, 0);
	current->runAction(orbitCur);

	//move next
	//next->stopAllActions();
	next->setPosition(ccp(winSize.width + 30, 30));
	next->setVisible(true);
	//
	CCMoveTo *moveNext = CCMoveTo::create(2, ccp(30, 30));
	next->runAction(moveNext);
	//
	CCOrbitCamera *orbitNext = CCOrbitCamera::create(2, 1, 0, 90, -90, 0, 0);
	next->runAction(orbitNext);
}
