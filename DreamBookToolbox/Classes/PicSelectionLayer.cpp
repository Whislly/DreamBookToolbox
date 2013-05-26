#include "PicSelectionLayer.h"

PicSelectionLayer::PicSelectionLayer(void)
{
	this->elementArray = NULL;
	this->selectArray = NULL;
}

PicSelectionLayer::~PicSelectionLayer(void)
{
	if (this->elementArray != NULL)
	{
		delete this->elementArray;
	}
	if (this->selectArray != NULL)
	{
		delete this->selectArray;
	}
}

void PicSelectionLayer::update(float dt)
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

void PicSelectionLayer::draw()
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

	CCSize size = this->getContentSize();
	ccDrawRect(ccp(0,0), ccp(size.width, size.height));
}

bool PicSelectionLayer::init()
{
	//
	this->elementArray = new CCArray();
	this->selectArray = new CCArray();
	this->ignoreAnchorPointForPosition(true);

	//
	this->setTouchEnabled(true);

	//
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	this->setContentSize(CCSize(240, winSize.height - 30 - 130));

	//combine time button
	CCMenuItemImage *combineItem = CCMenuItemImage::create(
		"Images/PicPickup/combineT.png",
		"Images/PicPickup/combineT.png",
		this,
		menu_selector(PicSelectionLayer::ClickOnCombineTime));
	combineItem->setScale(0.5);
	combineItem->setAnchorPoint(CCPointZero);
	combineItem->setPosition(20,  -combineItem->getContentSize().height / 4);
	CCMenu* menu = CCMenu::create(combineItem, NULL);
	menu->setPosition(0, 0);
	this->addChild(menu);

	return true;
}

//=============================== Touches ===============================
//Begin
void PicSelectionLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	this->currentSelect = NULL;

	CCSetIterator it;
	CCTouch* touch;

	for ( it = pTouches->begin(); it != pTouches->end(); it++)
	{
		touch = (CCTouch*)(*it);

		if(!touch)
			break;

		CCPoint location = touch->getLocation();
		location.x -= this->getPositionX();
		location.y -= this->getPositionY();

		for (int i = 0; i < this->elementArray->count(); i++)
		{
			CCSprite *s = (CCSprite*)this->elementArray->objectAtIndex(i);
			CCRect rect = CCRect(
				s->getPositionX(), s->getPositionY(),
				s->getContentSize().width, s->getContentSize().height);
			//selected
			if (rect.containsPoint(location))
			{
				this->currentSelect = s;
				break;
			}
		}
	}
}

void PicSelectionLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
}

void PicSelectionLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	if (this->currentSelect == NULL)
	{
		return;
	}

	//Add a new body/atlas sprite at the touched location
	CCSetIterator it;
	CCTouch* touch;

	for ( it = pTouches->begin(); it != pTouches->end(); it++)
	{
		touch = (CCTouch*)(*it);

		if(!touch)
			break;

		CCPoint location = touch->getLocation();
		location.x -= this->getPositionX();
		location.y -= this->getPositionY();

		CCSprite* selectEnd = NULL;
		for (int i = 0; i < this->elementArray->count(); i++)
		{
			CCSprite *s = (CCSprite*)this->elementArray->objectAtIndex(i);
			CCRect rect = CCRect(
				s->getPositionX(), s->getPositionY(),
				s->getContentSize().width, s->getContentSize().height);
			//selected
			if (rect.containsPoint(location))
			{
				selectEnd = s;
				break;
			}
		}

		//click
		if (selectEnd == this->currentSelect)
		{
			bool selected = this->selectArray->containsObject(this->currentSelect);
			//un-select
			if (selected)
			{
				this->selectArray->removeObject(this->currentSelect);
				this->currentSelect->setRotation(0.0F);
				this->currentSelect->stopActionByTag(1);
			}
			//select
			else
			{
				this->selectArray->addObject(this->currentSelect);
				CCActionInterval *rot1 = CCRotateBy::create(0.25F, 15);
				CCActionInterval *rot2 = rot1->reverse();
				CCFiniteTimeAction *seq = CCSequence::create(
					rot1,
					rot2,
					rot2,
					rot1,
					NULL);
				CCRepeatForever *repeatAction = CCRepeatForever::create((CCActionInterval*)seq);
				repeatAction->setTag(1);
				this->currentSelect->runAction(repeatAction);
			}
		}
	}

	this->currentSelect = NULL;
}

void PicSelectionLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
}

//End

CCPoint PicSelectionLayer::GetAdjustPos(CCSize spriteSize, int currentIndex)
{
	int margin = 10;
	int interval = 10;
	CCSize contentSize = this->getContentSize();

	//set position
	CCSize num = CommonHelper::CountContainerNumber(contentSize, spriteSize.width, interval, margin, spriteSize.height, interval, margin);
	int count = this->elementArray->count();
	int posXIndex = currentIndex % ((int)num.width);
	int posYIndex = currentIndex / ((int)num.width);

	return ccp(margin + (interval + spriteSize.width) * posXIndex, contentSize.height - margin - (interval + spriteSize.height) * posYIndex - spriteSize.height);
}

CCPoint PicSelectionLayer::GetPreAddPos(CCSize spriteSize)
{
	int count = this->elementArray->count();
	return this->GetAdjustPos(spriteSize, count);
}

//flow layout here
void PicSelectionLayer::AddElement(CCSprite *sprite)
{
	CCPoint pos = this->GetPreAddPos(sprite->getContentSize());
	sprite->setAnchorPoint(ccp(0, 0));
	sprite->setPosition(pos);

	//add it
	this->addChild(sprite);
	this->elementArray->addObject(sprite);
}

FrameSprite* PicSelectionLayer::CombineSpriteOnTime(CCArray* spriteArray, float interval)
{
	CCSprite *sprite = (CCSprite*)spriteArray->objectAtIndex(0);
	FrameSprite *frameSprite = FrameSprite::create();
	CommonHelper::CopySpriteProperty(frameSprite, sprite);

	CCArray* animFrames = CCArray::createWithCapacity(spriteArray->count());
	for (int i = 0; i < spriteArray->count(); i++)
	{
		FileSprite *sprite = (FileSprite*)spriteArray->objectAtIndex(i);
		CCTexture2D *texture = sprite->getTexture();
		CCSize size = texture->getContentSize();
		CCSpriteFrame *frame = CCSpriteFrame::createWithTexture(texture, CCRectMake(0, 0, size.width, size.height));
		frame->setOriginalSize(CCSize(size.width * sprite->getScale(), size.height * sprite->getScale()));
		animFrames->addObject(frame);

		frameSprite->AddPath(sprite->path);
	}

	CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, interval);

	CCAnimate *animate = CCAnimate::create(animation);
	CCRepeatForever *repeatAction =  CCRepeatForever::create( animate );

	frameSprite->runAction(repeatAction);

	return frameSprite;
}

void PicSelectionLayer::AdjustSprite(CCArray *spriteArray, CCSprite *newSprite)
{
	int min = 100000;
	for (int i = 0; i < spriteArray->count(); i++)
	{
		CCSprite *sprite = (CCSprite*)spriteArray->objectAtIndex(i);
		int index = this->elementArray->indexOfObject(sprite);
		if (index < min)
		{
			min = index;
		}
	}

	//1. remove
	for (int i = 0; i < spriteArray->count(); i++)
	{
		CCSprite *sprite = (CCSprite*)spriteArray->objectAtIndex(i);
		this->removeChild(sprite, true);
		this->elementArray->removeObject(sprite, true);
	}

	//2. add new (move to the min position)
	this->elementArray->insertObject(newSprite, min);
	this->addChild(newSprite);

	//3. adjust all sprite's position
	for (int i = 0; i < this->elementArray->count(); i++)
	{
		CCSprite *sprite = (CCSprite*)this->elementArray->objectAtIndex(i);
		CCSize size = sprite->getContentSize();
		CCPoint pos = this->GetAdjustPos(size, i);

		//Move action here.
		if (sprite == newSprite)
		{
			sprite->setPosition(pos);
		}
		else
		{
			CCMoveTo *moveTo = CCMoveTo::create(0.25F, pos);
			sprite->runAction(moveTo);
		}
	}
}

void PicSelectionLayer::ClickOnCombineTime(CCObject* pSender)
{
	int count = this->selectArray->count();
	if (count <= 0)
	{
		return;
	}

	CCSprite *newSprite = this->CombineSpriteOnTime(this->selectArray, 0.25F);
	this->AdjustSprite(this->selectArray, newSprite);

	//TODO here : Shadow - Patch
	newSprite->setPositionX(newSprite->getPositionX() + 16);
	newSprite->setPositionY(newSprite->getPositionY() + 16);

	this->selectArray->removeAllObjects();
}

CCArray* PicSelectionLayer::getElementArray()
{
	return this->elementArray;
}