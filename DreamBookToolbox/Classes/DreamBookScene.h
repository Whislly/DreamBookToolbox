#ifndef __DreamBook_SCENE_H__
#define __DreamBook_SCENE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"
#include "DesignLayer.h"
#include "ObserveLayer.h"
#include "PicPickupLayer.h"

#include "Parse\ParseError.h"
#include "Parse\ParseFile.h"

#include "Parse.h"
#include "Delegate.h"
#include "ParseObject.h"
#include "ParseFile.h"
#include "ParseQuery.h"


#define    RAND_LIMIT    32767

class DreamBookLayer : public cocos2d::CCLayer
{
private:
	cocos2d::CCParticleSystem* createGestureStyle();
    void gestureResult(const char* name, double score);

    inline float32 RandomFloat(float32 lo, float32 hi)
    {
        float32 r = (float32)(std::rand() & (RAND_LIMIT));
        r /= RAND_LIMIT;
        r = (hi - lo) * r + lo;
        return r;
    }

    ObserveLayer* m_observeLayer;
    DesignLayer* m_designLayer;
    PicPickupLayer* m_picPickupLayer;
    cocos2d::extension::ParseFile* file;
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();    
    void addBackground();
    void activeActions(CCObject* pSender);
    void activeCell(CCObject* pSender);
    void showPicPickupLayer(CCObject* pSender);
    void addNewCell(cocos2d::CCObject* pSender, cocos2d::CCArray* pElementArray);
    cocos2d::CCSprite* getDefaultCell();
    void addObserveLayer();
    void addDesignLayer();

    // a selector callback
    void menuBackCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(DreamBookLayer);

    void saveDataToCloud(CCObject* pSender);
    void readyUploadFile();

    void UploadComplet(cocos2d::extension::FileInfo* fileInfo, cocos2d::extension::ParseError* error);
    void DownloadFile();
    void DownloadComplet(cocos2d::CCArray* array, cocos2d::extension::ParseError* error);

    DreamBookLayer();
    ~DreamBookLayer();
};

#endif  // __DreamBook_SCENE_H__