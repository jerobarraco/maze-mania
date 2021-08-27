LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES :=\
../../Classes/AppDelegate.cpp \
../../Classes/utils/Utils.cpp \
../../Classes/utils/events.cpp \
../../Classes/utils/Files.cpp \
../../Classes/items/Item.cpp \
../../Classes/items/Point.cpp \
../../Classes/items/Bomb.cpp \
../../Classes/items/GameTimer.cpp \
../../Classes/items/wavetimer.cpp \
../../Classes/items/Goal.cpp \
../../Classes/items/Speed.cpp \
../../Classes/TouchSprite.cpp \
../../Classes/GameOverMenu.cpp \
../../Classes/menu/BtnPause.cpp \
../../Classes/menu/BtnUnpause.cpp \
../../Classes/menu/spinbox.cpp \
../../Classes/menu/btnurl.cpp \
../../Classes/menu/btnback.cpp \
../../Classes/menu/btnevent.cpp \
../../Classes/menu/overlay.cpp \
../../Classes/Board.cpp \
../../Classes/Door.cpp \
../../Classes/DevMenu.cpp \
../../Classes/StartMenu.cpp \
../../Classes/Player.cpp \
../../Classes/scenes/main_scene.cpp \
../../Classes/scenes/exit.cpp \
../../Classes/scenes/game.cpp \
../../Classes/scenes/about.cpp \
../../Classes/scenes/loader.cpp \
../../Classes/scenes/intro.cpp \
../../Classes/scenes/help.cpp \
../../Classes/anim/animation.cpp \
../../Classes/anim/aitem.cpp \
main.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
#LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
#LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
#LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
#$(call import-module,extensions)
#$(call import-module,external/Box2D)
#$(call import-module,external/chipmunk)
