#ifndef Define_Common
#define Define_Common

#define PTM_RATIO 32
#define FocusActionTag 999
#define Size_N1 64
#define Size_N2 128
#define Size_N3 256

#define Pic_Count 65

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


#define CCARRAY_RELEASE(__array__)                                                                                             \
    if ((__array__))                                                                                                           \
    {                                                                                                                          \
        if((__array__)->data->num > 0)                                                                                         \
        {                                                                                                                      \
            CCObject* __object__ = NULL;                                                                                       \
            for(CCObject** __arr__ = (__array__)->data->arr, **__end__ = (__array__)->data->arr + (__array__)->data->num-1;    \
            __arr__ <= __end__ && (((__object__) = *__arr__) != NULL/* || true*/);                                             \
            __arr__++)                                                                                                         \
            {                                                                                                                  \
                if ((__object__))                                                                                              \
                {                                                                                                              \
                    (__object__)->release();                                                                                   \
                }                                                                                                              \
            }                                                                                                                  \
        }                                                                                                                      \
        (__array__)->release();                                                                                                \
        (__array__) = NULL;                                                                                                    \
    }
#endif