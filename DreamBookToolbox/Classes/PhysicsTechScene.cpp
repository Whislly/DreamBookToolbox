#include "PhysicsTechScene.h"

#include "MenuLayer.h"
#include "PropertyLayer.h"
#include "PhysicsTechLayer.h"

USING_NS_CC;

bool PhysicsTechScene::init()
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

        bRet = true;
    } while (0);

    return bRet;
}