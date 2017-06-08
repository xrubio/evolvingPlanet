# Application.mk used by the android studio build
# Copy or link to EvolvingPlanet//proj.android-studio/app/jni/

APP_STL := c++_shared

APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char
APP_LDFLAGS := -latomic

APP_ABI := armeabi
APP_SHORT_COMMANDS := true


ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif
