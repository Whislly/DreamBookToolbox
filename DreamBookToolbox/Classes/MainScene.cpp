#include "MainScene.h"
#include "PhysicsTechScene.h"
#include "DreamBookScene.h"
#include "ToyBrickScene.h"
#include "DancingBoneScene.h"
#include "Parse.h"
#include "Delegate.h"
#include "ParseObject.h"
#include "ParseFile.h"
#include "ParseQuery.h"

USING_NS_CC;

using namespace cocos2d::extension;

void MainLayer::menuDreamBookCallback( CCObject* pSender )
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0f, DreamBookLayer::scene(), false));
}

void MainLayer::menuPhysicsTechCallback( CCObject* pSender )
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0f, PhysicsTechScene::create(), false));
}

void MainLayer::menuToyBrickCallback( CCObject* pSender )
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0f, ToyBrickScene::create(), false));
}

void MainLayer::menuDancingBoneCallback( CCObject* pSender )
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0f, DancingBoneScene::create(), false));
}

void MainLayer::menuCloseCallback( CCObject* pSender )
{
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->removeSpriteFramesFromFile("PlayBox.plist");

    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

//void MainLayer::UploadComplet(FileInfo* fileInfo, ParseError* error)
//{	
//	//obj->toString(str);
//	//this->lbDebug->setString("SaveComplet start");
//	static char buffer[4096] = {0};
//	if (fileInfo)
//	{
//		sprintf(buffer, "%s, %s:", fileInfo->GetUrl().c_str(), fileInfo->GetFileName().c_str());
//	}
//
//	//this->lbDebug->setString((buffer + error->GetError()).c_str());
//}
//
////void MainLayer::SaveComplet(bool success, ParseError* error)
//{	
//	//obj->toString(str);
//	this->lbDebug->setString("SaveComplet start");
//
//	static char buffer[4096] = {0};
//	sprintf(buffer, "%s, error:", success?"success":"faiture");
//
//	this->lbDebug->setString((buffer + error->GetError()).c_str());
//}
//
//void MainLayer::GetComplet(CCArray* array, ParseError* error)
//{
//	if (array && array->count() > 0)
//	{
//		this->lbDebug->setString("array strat");
//
//		cocos2d::extension::ParseObject* obj = (cocos2d::extension::ParseObject*)array->objectAtIndex(0);
//			
//		std::string str;
//		obj->toString(str);
//
//		this->lbDebug->setString("get count");
//		int age = obj->Get<int>("count");
//
//		this->lbDebug->setString("set count");
//		obj->Set("count", age + 1);
//
//		this->lbDebug->setString("save()");
//		obj->saveCompleted.Set(this, (Delegate<bool, ParseError*>::MemberFun)&MainLayer::SaveComplet);
//		obj->save();
//
//		array->removeAllObjects();
//	}
//	else
//	{
//		this->lbDebug->setString(error->GetError().c_str());
//
//		cocos2d::extension::ParseObject* gameTest = new cocos2d::extension::ParseObject("GameTest");
//		gameTest->Add("name", "shadow");
//		gameTest->Add("count", 1);
//		gameTest->Add("age", 29);
//		gameTest->Add("score", 80);
//		gameTest->save();
//		gameTest->saveCompleted.Set(this, (Delegate<bool, ParseError*>::MemberFun)&MainLayer::SaveComplet);
//		gameTest->release();
//	}
//}

bool MainLayer::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        cache->addSpriteFramesWithFile("PlayBox.plist", "PlayBox.png");

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemSprite *pCloseItem = CCMenuItemSprite::create(
            CCSprite::createWithSpriteFrameName("close.png"), NULL, this,
            menu_selector(MainLayer::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        CCSize winSize = CCDirector::sharedDirector()->getWinSize();

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(winSize.width - 64, winSize.height - 64));
        pCloseItem->setScale(2.0f);

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to Main layer as a child layer.
        this->addChild(pMenu, 1);

        CCLabelTTF* label = CCLabelTTF::create("DreamBook", "Arial", 46);
        CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(MainLayer::menuDreamBookCallback));
        pMenu = CCMenu::create(pMenuItem, NULL);

        label = CCLabelTTF::create("PhysicsTech", "Arial", 46);
        pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(MainLayer::menuPhysicsTechCallback));
        pMenu->addChild(pMenuItem);

        label = CCLabelTTF::create("ToyBrick", "Arial", 46);
        pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(MainLayer::menuToyBrickCallback));
        pMenu->addChild(pMenuItem);

        label = CCLabelTTF::create("DancingBone", "Arial", 46);
        pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(MainLayer::menuDancingBoneCallback));
        pMenu->addChild(pMenuItem);

        pMenu->alignItemsVertically();
        this->addChild(pMenu);

        pMenu->setPosition(ccp(winSize.width * 0.5F, winSize.height * 0.7F));

		/*this->lbDebug = CCLabelTTF::create("Start", "Arial", 24);
		this->lbDebug->setColor(ccc3(255, 255, 1));
		this->lbDebug->setPosition(ccp(0, winSize.height - 100));
		this->lbDebug->setAnchorPoint(CCPointZero);
		this->addChild(this->lbDebug);*/

		//add Whislly test start
		/*cocos2d::extension::Parse parse;
		parse.setApplicationId("n1s82tGoQDgDM09qFNm0UQkKNO7yW1gqpQSavT5n");
		parse.setApiKey("Nh4K6HM2tohkmQdeu5vfN7ZWP83OMQ72uH0YMtW7");
		parse.setMasterKey("doMxP88XvGkICRrI3gPcCdwWMI26QZfzPMKER33m");*/

		//ParseFile* file = new ParseFile();
		/*file->uploadFileCompleted.Set(this, (Delegate<FileInfo*, ParseError*>::MemberFun)&MainLayer::UploadComplet);
		file->uploadFile("D:", "smbios.zip");*/
		//file->downloadFile("http://files.parse.com/148a2d0f-6637-47cf-b4d1-0c2083de23ef/03e0430a-ee2f-4e36-81fa-48c91607afa7-smbios.zip","D:\\Test.zip");
		//file->deleteFile("03e0430a-ee2f-4e36-81fa-48c91607afa7-smbios.zip");
		//file->release();
		/*
		cocos2d::extension::ParseQuery* query = new cocos2d::extension::ParseQuery("GameTest");
		query->findObjectsCompleted.Set(this, (Delegate<CCArray*, ParseError*>::MemberFun)&MainLayer::GetComplet);
		query->findObjects();
		query->release();*/
        CC_BREAK_IF(! pMenu);

        bRet = true;
    } while (0);

    return bRet;
}

cocos2d::CCScene* MainLayer::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        MainLayer *layer = MainLayer::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}