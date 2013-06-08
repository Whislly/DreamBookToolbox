#include "EFTestLayer.h"

EFTestLayer::EFTestLayer(void)
{
}

EFTestLayer::~EFTestLayer()
{
}

bool EFTestLayer::init()
{
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
	left->setPosition(ccp(400, 100));
	this->addChild(left);

	CCSprite *right = CCSprite::create("Images/EFTest/right.png");
	right->setUserData(rightDic);
	right->setPosition(ccp(460, 200));
	this->addChild(right);

	CCArray *arr = CCArray::create();
	arr->addObject(right);

	//if match, do the action
	DockTogetherSpore *spore = new DockTogetherSpore();
	spore->Do(left, arr, "ABC");

	return true;
}


void EFTestLayer::menuBackCallback( CCObject* pSender )
{
	CCDirector::sharedDirector()->replaceScene(MainLayer::scene());
}



