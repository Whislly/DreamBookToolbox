#include "DancingBoneScene.h"

DancingBoneScene::DancingBoneScene(void)
{
}

DancingBoneScene::~DancingBoneScene(void)
{
}

bool DancingBoneScene::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCScene::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        DancingBoneLayer *mainLayer = DancingBoneLayer::create();
        this->addChild(mainLayer);

		DancingBoneMenuLayer *menuLayer = DancingBoneMenuLayer::create();
		menuLayer->mainLayer = mainLayer;
		this->addChild(menuLayer);

        bRet = true;
    } while (0);

    return bRet;
}
