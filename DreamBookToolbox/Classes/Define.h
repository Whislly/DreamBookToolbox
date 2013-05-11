#ifndef Define_Common
#define Define_Common

#define PTM_RATIO 32

enum ShapeType
{
		ShapeType_None,
		Polygon_Shape,
		Circle_Shape
};

enum PropertyType
{
		PropertyType_None,
		PropertyType_Static,
		PropertyType_Mass,
		PropertyType_Impulse
};

#define CREATE_WITH_WORLD(__TYPE__) \
static __TYPE__* create(b2World* world) \
{ \
		__TYPE__ *pRet = new __TYPE__(); \
		pRet->world = world; \
		if (pRet->init()) \
		{ \
				pRet->autorelease(); \
				return pRet; \
		} \
		else \
		{ \
				delete pRet; \
				pRet = NULL; \
				return NULL; \
		} \
}

typedef void (cocos2d::CCObject::*SEL_ImportElementsHandler)(cocos2d::CCObject* pSender, cocos2d::CCArray* pElementArray);
#define importElement_selector(_SELECTOR) (SEL_ImportElementsHandler)(&_SELECTOR)

#endif