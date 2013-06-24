LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/CCPlaceInterval.cpp \
                   ../../Classes/CCSpriteEx.cpp \
                   ../../Classes/CCTransitionParticle.cpp \
                   ../../Classes/CommonHelper.cpp \
                   ../../Classes/CustomUserData.cpp \
                   ../../Classes/DBActionSprite.cpp \
                   ../../Classes/DBData.cpp \
                   ../../Classes/DBPropertyData.cpp \
                   ../../Classes/DancingBoneLayer.cpp \
                   ../../Classes/DancingBoneMenuLayer.cpp \
                   ../../Classes/DancingBoneScene.cpp \
                   ../../Classes/DesignLayer.cpp \
                   ../../Classes/DockMoveSpore.cpp \
                   ../../Classes/DockTogetherSpore.cpp \
                   ../../Classes/DreamBookScene.cpp \
                   ../../Classes/DynamicLayerContainer.cpp \
                   ../../Classes/EFTestLayer.cpp \
                   ../../Classes/EFTestScene.cpp \
                   ../../Classes/FileSprite.cpp \
                   ../../Classes/FrameSprite.cpp \
                   ../../Classes/GeometricRecognizer.cpp \
                   ../../Classes/GestureLayer.cpp \
                   ../../Classes/KeyboardNotificationLayer.cpp \
                   ../../Classes/MainScene.cpp \
                   ../../Classes/MenuLayer.cpp \
                   ../../Classes/MultiBodyQueryCallback.cpp \
                   ../../Classes/ObserveLayer.cpp \
                   ../../Classes/ObserveLayerContainer.cpp \
                   ../../Classes/PhysicalBodyScene.cpp \
                   ../../Classes/PhysicsSprite.cpp \
                   ../../Classes/PhysicsTechLayer.cpp \
                   ../../Classes/PhysicsTechScene.cpp \
                   ../../Classes/PicPickupLayer.cpp \
                   ../../Classes/PicSelectionLayer.cpp \
                   ../../Classes/PicShowLayer.cpp \
                   ../../Classes/PosQuery.cpp \
                   ../../Classes/PositionQueryCallback.cpp \
                   ../../Classes/PropertyLayer.cpp \
                   ../../Classes/StoryChatLayer.cpp \
                   ../../Classes/StoryScene.cpp \
                   ../../Classes/ToyBrick.cpp \
                   ../../Classes/ToyBrickScene.cpp \
                   ../../Classes/ToyContact.cpp \
                   ../../Classes/ToyLayer.cpp \
                   ../../Classes/VisibleRect.cpp \


                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../../external \
					$(LOCAL_PATH)/../../../external/Box2D \
					$(LOCAL_PATH)/../../../extensions \
					$(LOCAL_PATH)/../../../cocos2dx \
					$(LOCAL_PATH)/../../../cocos2dx/cocoa \
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
								cocos_libssl_static \
								cocos_libcrypto_static \
								libgnustl_static \
								chipmunk_static
LOCAL_LDLIBS += -lz
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions) \
$(call import-module,external/Box2D) \
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libssl)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcrypto)
