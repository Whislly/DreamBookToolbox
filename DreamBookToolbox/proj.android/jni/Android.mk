LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/Box2DLayer.cpp \
                   ../../Classes/CommonHelper.cpp \
                   ../../Classes/CustomUserData.cpp \
                   ../../Classes/DynamicLayerContainer.cpp \
                   ../../Classes/GeometricRecognizer.cpp \
                   ../../Classes/JointTestLayer.cpp \
                   ../../Classes/KeyboardNotificationLayer.cpp \
                   ../../Classes/MenuLayer.cpp \
                   ../../Classes/MyLayer.cpp \
                   ../../Classes/ParticleLayer.cpp \
                   ../../Classes/PhysicsSprite.cpp \
                   ../../Classes/PhysicsTechLayer.cpp \
                   ../../Classes/PicPickupLayer.cpp \
                   ../../Classes/PicSelectionLayer.cpp \
                   ../../Classes/PicShowLayer.cpp \
                   ../../Classes/PosQuery.cpp \
                   ../../Classes/PositionQueryCallback.cpp \
                   ../../Classes/PropertyLayer.cpp \
                   ../../Classes/ToyBrick.cpp \
                   ../../Classes/ToyContact.cpp \
                   ../../Classes/ToyLayer.cpp \
                   ../../Classes/VisibleRect.cpp

                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../../external \
					$(LOCAL_PATH)/../../../external/Box2D \
					$(LOCAL_PATH)/../../../extensions \
					$(LOCAL_PATH)/../../../cocos2dx \
					$(LOCAL_PATH)/../../../cocos2dx/include \
					$(LOCAL_PATH)/../../../cocos2dx/kazmath/include \
					$(LOCAL_PATH)/../../../cocos2dx/platform/android \
					$(LOCAL_PATH)/../../../cocos2dx/platform/third_party/android \
					$(LOCAL_PATH)/../../../cocos2dx/platform/third_party/android/OGLES \
					$(LOCAL_PATH)/../../../chipmunk/include/chipmunk \
					$(LOCAL_PATH)/../../../CocosDenshion/include 


LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static \
								cocosdenshion_static \
								cocos_extension_static \
								box2d_static \
								cocos_curl_static \
								cocos_png_static \
								cocos_tiff_static \
								cocos_xml2_static \
								libgnustl_static \
								chipmunk_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions) \
$(call import-module,external/box2d) \
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
