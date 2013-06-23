#include "PhysicsTechScene.h"

PhysicsTechScene::PhysicsTechScene(void)
{
}

PhysicsTechScene::~PhysicsTechScene()
{
}

bool PhysicsTechScene::init()
{
	b2World* world = CommonHelper::CreateWorld();

	//Menu Layer
	MenuLayer *menu = MenuLayer::create(world);
	this->addChild(menu, 0);

	//Property Layer
	PropertyLayer *prop = PropertyLayer::create(world);
	this->addChild(prop, 2);

	//Main Layer
	PhysicsTechLayer *tcLayer = PhysicsTechLayer::create(world);
	tcLayer->propertyLayer = prop;
	this->addChild(tcLayer, 1);

	if(CCDirector::sharedDirector()->isPaused())
	{
		CCDirector::sharedDirector()->resume();
	}

	return true;
}