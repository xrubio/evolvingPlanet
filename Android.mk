# Android.mk used by the android studio build
# Copy or link to EvolvingPlanet//proj.android-studio/app/jni/

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../../Classes/Achievement.cpp \
                   ../../../Classes/DispersalGoal.cpp \
                   ../../../Classes/Goal.cpp \
                   ../../../Classes/LevelLoader.cpp \
                   ../../../Classes/Power.cpp \
                   ../../../Classes/Terraform.cpp \
                   ../../../Classes/UIAchievements.cpp \
                   ../../../Classes/UIEndScene.cpp \
                   ../../../Classes/UIIntroStory.cpp \
                   ../../../Classes/UIProgressMap.cpp \
                   ../../../Classes/Agent.cpp \
                   ../../../Classes/Exploit.cpp \
                   ../../../Classes/Influence.cpp \
                   ../../../Classes/LocalizedString.cpp \
                   ../../../Classes/ProgressAchievement.cpp \
                   ../../../Classes/Timing.cpp \
                   ../../../Classes/UIAreaPower.cpp \
                   ../../../Classes/UIGameplayMap.cpp \
                   ../../../Classes/UIIntroToast.cpp \
                   ../../../Classes/UIStoryGallery.cpp \
                   ../../../Classes/AppDelegate.cpp \
                   ../../../Classes/GameData.cpp \
                   ../../../Classes/Kill.cpp \
                   ../../../Classes/Message.cpp \
                   ../../../Classes/Reproduce.cpp \
                   ../../../Classes/Trade.cpp \
                   ../../../Classes/UIConfiguration.cpp \
                   ../../../Classes/UIGlobalPower.cpp \
                   ../../../Classes/UIMainMenu.cpp \
                   ../../../Classes/UITransitionScene.cpp \
                   ../../../Classes/AreaPower.cpp \
                   ../../../Classes/GameLevel.cpp \
                   ../../../Classes/LevelAchievement.cpp \
                   ../../../Classes/Position.cpp \
                   ../../../Classes/ResourcesGoal.cpp \
                   ../../../Classes/Tutorial.cpp \
                   ../../../Classes/UICredits.cpp \
                   ../../../Classes/UIGoals.cpp \
                   ../../../Classes/UIPower.cpp \
                   ../../../Classes/WaveNode.cpp \
                   ../../../libs/pugixml/pugixml.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
                    $(LOCAL_PATH)/../../../libs/pugixml

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END

