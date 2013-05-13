#include "ToyLayer.h"
#include "ToyBrickScene.h"

USING_NS_CC;

bool ToyBrickScene::init()
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

        ToyLayer* toyLayer = ToyLayer::create();
        this->addChild(toyLayer);

        bRet = true;
    } while (0);

    return bRet;
}