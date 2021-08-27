EXECUTABLE = mazemania

INCLUDES = -I.. -I../Classes
OBJ_DIR ?= obj
HEADERS = \
	../Classes/AppMacros.h \

SOURCES = \
../Classes/Utils.cpp \
../Classes/Event.cpp \
../Classes/AppDelegate.cpp \
../Classes/items/Item.cpp \
../Classes/items/Bomb.cpp \
../Classes/items/GameTimer.cpp \
../Classes/items/Goal.cpp \
../Classes/TouchSprite.cpp \
../Classes/GameOverMenu.cpp \
../Classes/menu/BtnPause.cpp \
../Classes/menu/BtnUnpause.cpp \
../Classes/Board.cpp \
../Classes/Door.cpp \
../Classes/MainMenu.cpp \
../Classes/MainScene.cpp \
../Classes/Player.cpp \
main.cpp \


OBJECTS := $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)
OBJECTS := $(addprefix $(OBJ_DIR)/, $(OBJECTS))
DEPS = $(OBJECTS:.o=.d)
-include $(DEPS)
#include mazemania.mk

COCOS_ROOT = ../../..
include $(COCOS_ROOT)/cocos2dx/proj.linux/cocos2dx.mk

SHAREDLIBS += -lcocos2d
COCOS_LIBS = $(LIB_DIR)/libcocos2d.so

$(TARGET): $(OBJECTS) $(STATICLIBS) $(COCOS_LIBS) $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_LINK)$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(SHAREDLIBS) $(STATICLIBS)

$(OBJ_DIR)/%.o: %.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

$(OBJ_DIR)/%.o: ../%.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@
