EXECUTABLE = mazemania

INCLUDES = -I.. -I../Classes -I../../../CocosDenshion/include
OBJ_DIR ?= obj
HEADERS = \
../Classes/AppMacros.h \
#from build.sh
#DEPENDS='libx11-dev' DEPENDS+=' libxmu-dev' DEPENDS+=' libglu1-mesa-dev' DEPENDS+=' libgl2ps-dev' DEPENDS+=' libxi-dev' DEPENDS+=' libglfw-dev'  DEPENDS+=' g++' DEPENDS+=' libzip-dev' DEPENDS+=' libcurl4-gnutls-dev' DEPENDS+=' libfontconfig1-dev' DEPENDS+=' libsqlite3-dev'

SOURCES = \
main.cpp \
../Classes/AppDelegate.cpp \
../Classes/utils/Utils.cpp \
../Classes/utils/events.cpp \
../Classes/utils/Files.cpp \
../Classes/items/Item.cpp \
../Classes/items/Bomb.cpp \
../Classes/items/GameTimer.cpp \
../Classes/items/wavetimer.cpp \
../Classes/items/Goal.cpp \
../Classes/items/Speed.cpp \
../Classes/items/Point.cpp \
../Classes/TouchSprite.cpp \
../Classes/GameOverMenu.cpp \
../Classes/menu/BtnPause.cpp \
../Classes/menu/BtnUnpause.cpp \
../Classes/menu/spinbox.cpp \
../Classes/menu/btnurl.cpp \
../Classes/menu/btnevent.cpp \
../Classes/menu/overlay.cpp \
../Classes/Board.cpp \
../Classes/Door.cpp \
../Classes/DevMenu.cpp \
../Classes/StartMenu.cpp \
../Classes/Player.cpp \
../Classes/scenes/main_scene.cpp \
../Classes/scenes/exit.cpp \
../Classes/scenes/game.cpp \
../Classes/scenes/about.cpp \
../Classes/scenes/loader.cpp \
../Classes/scenes/intro.cpp \
../Classes/scenes/help.cpp \
../Classes/anim/animation.cpp \
../Classes/anim/aitem.cpp \


OBJECTS := $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)
OBJECTS := $(addprefix $(OBJ_DIR)/, $(OBJECTS))
DEPS = $(OBJECTS:.o=.d)
-include $(DEPS)
include mazemania.mk

COCOS_ROOT = ../../..
#include $(COCOS_ROOT)/CocosDenshion/proj.$(PLATFORM)/cocosdenshion.mk
include $(COCOS_ROOT)/cocos2dx/proj.linux/cocos2dx.mk

SHAREDLIBS += -lcocos2d -lcocosdenshion
#SHAREDLIBS += -l/usr/lib/x86_64-linux-gnu/libGLEW.so.1.8
#STATICLIBS += $(LIB_DIR)/libcocos2d.so
#SHAREDLIBS += /usr/lib/x86_64-linux-gnu/libGLEW.so.1.8
COCOS_LIBS = $(LIB_DIR)/libcocos2d.so
LDFLAGS = -static

$(TARGET): $(OBJECTS) $(STATICLIBS) $(COCOS_LIBS) $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_LINK)$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(SHAREDLIBS) $(STATICLIBS)

$(OBJ_DIR)/%.o: %.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

$(OBJ_DIR)/%.o: ../%.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@
