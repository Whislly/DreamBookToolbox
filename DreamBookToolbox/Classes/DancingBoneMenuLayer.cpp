#include "DancingBoneMenuLayer.h"

DancingBoneMenuLayer::DancingBoneMenuLayer(void)
{
		this->startPauseItem = NULL;
		this->startImage = NULL;
		this->pauseImage = NULL;
}

DancingBoneMenuLayer::~DancingBoneMenuLayer()
{
		if (this->startImage != NULL)
		{
				this->startImage->release();
		}
		if (this->pauseImage != NULL)
		{
				this->pauseImage->release();
		}
}

bool DancingBoneMenuLayer::init()
{
		//mode
		this->isRealMode = false;

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		//Menu - Start/Pause
		this->startPauseItem = CCMenuItemImage::create(
				"Images/PhysicsTech/start.png",
				"Images/PhysicsTech/start.png",
				this,
				menu_selector(DancingBoneMenuLayer::StartPause));
		this->startPauseItem->setAnchorPoint(CCPointZero);
		CCSize size = this->startPauseItem->getContentSize();
		this->startPauseItem->setPosition(winSize.width / 2 - size.width, 32);
		
		this->startImage = CCSprite::create("Images/PhysicsTech/start.png");
		this->startImage->retain();
		this->pauseImage = CCSprite::create("Images/PhysicsTech/pause.png");
		this->pauseImage->retain();

		//reset
		CCMenuItemImage *resetItem = CCMenuItemImage::create(
				"Images/PhysicsTech/reset.png",
				"Images/PhysicsTech/reset.png",
				this,
				menu_selector(DancingBoneMenuLayer::Reset));
		resetItem->setAnchorPoint(CCPointZero);
		size = resetItem->getContentSize();
		resetItem->setPosition(winSize.width / 2 + size.width, 32);

		//Return back
		CCMenuItemImage *returnItem = CCMenuItemImage::create(
				"b1.png",
				"b2.png",
				this,
				menu_selector(DancingBoneMenuLayer::ReturnBack));
		returnItem->setAnchorPoint(CCPointZero);
		size = returnItem->getContentSize();
		returnItem->setPosition(winSize.width / 2 + size.width * 2, 32);

		//Menu
		CCMenu* menu = CCMenu::create(startPauseItem, resetItem, returnItem, NULL);
		menu->setPosition(CCPointZero);
		this->addChild(menu, 1);

		return true;
}

void DancingBoneMenuLayer::update(float dt)
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

void DancingBoneMenuLayer::draw()
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

void DancingBoneMenuLayer::StartPause(CCObject* pSender)
{
		this->isRealMode = !this->isRealMode;
		this->mainLayer->SetMode(this->isRealMode);

		//Pause/Resume
		if (this->isRealMode)
		{
				this->startPauseItem->setNormalImage(this->pauseImage);
				this->startPauseItem->setSelectedImage(this->pauseImage);
		}
		else
		{
				this->startPauseItem->setNormalImage(this->startImage);
				this->startPauseItem->setSelectedImage(this->startImage);
		}
}

void DancingBoneMenuLayer::Reset(CCObject* pSender)
{
		CCDirector::sharedDirector()->replaceScene(DancingBoneScene::create());
}

void DancingBoneMenuLayer::ReturnBack(CCObject* pSender)
{
		CCDirector::sharedDirector()->replaceScene(MainLayer::scene());
}