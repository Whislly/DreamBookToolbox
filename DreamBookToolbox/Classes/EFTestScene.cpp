#include "EFTestScene.h"

EFTestScene::EFTestScene(void)
{
}

EFTestScene::~EFTestScene()
{
}

bool EFTestScene::init()
{
	EFTestLayer *layer = EFTestLayer::create();
	this->addChild(layer);

	return true;
}

