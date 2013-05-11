#include "MenuLayer.h"
#include "MainScene.h"
#include "PhysicsTechScene.h"

USING_NS_CC;

bool MenuLayer::init()
{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		//Menu -Close
		CCMenuItemImage *closeItem = CCMenuItemImage::create(
				"Images/PhysicsTech/close.png",
				"Images/PhysicsTech/close.png",
				this,
				menu_selector(MenuLayer::Back));
		closeItem->setAnchorPoint(CCPointZero);

		//Set pos
		CCSize size = closeItem->getContentSize();
		closeItem->setPosition(ccp(winSize.width - size.width - 10, winSize.height - size.height - 10));

		//closeItem->setScale(1.0F);
		//closeItem->setRotation(270.0F);
		//closeItem->setTag(1);

		//Menu - Start/Pause
		CCMenuItemImage *startPauseItem = CCMenuItemImage::create(
				"Images/PhysicsTech/pause.png",
				"Images/PhysicsTech/pause.png",
				this,
				menu_selector(MenuLayer::StartPause));
		startPauseItem->setAnchorPoint(CCPointZero);
		size = startPauseItem->getContentSize();
		startPauseItem->setPosition(winSize.width / 2 - size.width, 32);
		this->startPauseItem = startPauseItem;
		this->startImage = CCSprite::create("Images/PhysicsTech/start.png");
		this->startImage->retain();
		//this->startImage->setVisible(false);
		//this->addChild(this->startImage);
		this->pauseImage = CCSprite::create("Images/PhysicsTech/pause.png");
		this->pauseImage->retain();
		//this->pauseImage->setVisible(false);
		//this->addChild(this->pauseImage);

		CCMenuItemImage *resetItem = CCMenuItemImage::create(
				"Images/PhysicsTech/reset.png",
				"Images/PhysicsTech/reset.png",
				this,
				menu_selector(MenuLayer::Reset));
		resetItem->setAnchorPoint(CCPointZero);
		size = resetItem->getContentSize();
		resetItem->setPosition(winSize.width / 2 + size.width, 32);
		
		//Menu
		CCMenu* menu = CCMenu::create(closeItem, startPauseItem, resetItem, NULL);
		menu->setPosition(CCPointZero);
		//menu->setTag(1);
		this->addChild(menu, 1);

		return true;
}


void MenuLayer::Back(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(0.5f, MainLayer::scene(), true));

	if(CCDirector::sharedDirector()->isPaused())
	{
			CCDirector::sharedDirector()->resume();
	}
	//Restart
	/*CCScene *s = CCDirector::sharedDirector()->getRunningScene();
	CCNode *layer = s->getChildByTag(2);

	s->removeChildByTag(2, true);

	MyLayer *l = MyLayer::create();
	l->setTag(2);
	s->addChild(l);*/
}

void MenuLayer::StartPause(CCObject* pSender)
{
		//Pause/Resume
		if(CCDirector::sharedDirector()->isPaused())
		{
				this->startPauseItem->setNormalImage(this->pauseImage);
				this->startPauseItem->setSelectedImage(this->pauseImage);
				CCDirector::sharedDirector()->resume();
		}
		else
		{
				this->startPauseItem->setNormalImage(this->startImage);
				this->startPauseItem->setSelectedImage(this->startImage);
				CCDirector::sharedDirector()->pause(); 
		}
}

void MenuLayer::Reset(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(PhysicsTechScene::create());
}
