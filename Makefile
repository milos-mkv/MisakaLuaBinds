CXX      	 := g++
CPPFLAGS   	 := -std=c++20 -arch x86_64
LIBS 	 	 := -L./libs/lua -llua52 -L./libs/glfw/lib-x86_64 -lglfw3
INCLUDE      := -I./libs/lua/include -I./libs/glfw/include -I./include
FRAMEWORKS   := -framework Cocoa -framework OpenGL -framework IOKit

TARGET_NAME  := main.out
BIN_PATH 	 := bin
OBJ_PATH 	 := obj
SRC_PATH 	 := src
DBG_PATH 	 := debug
SCRIPTS_PAHT := scripts

TARGET 		 := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DEBUG := $(DBG_PATH)/$(TARGET_NAME)

SRC 		 := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ 		 := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_DEBUG 	 := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

DISTCLEAN_LIST 	:= $(OBJ) $(OBJ_DEBUG)
CLEAN_LIST 		:= $(TARGET) $(TARGET_DEBUG) $(DISTCLEAN_LIST) $(BIN_PATH)/$(SCRIPTS_PAHT)

default: makedir all

$(TARGET): $(OBJ)
	$(CXX) $(CPPFLAGS) -o $@ $(OBJ) $(LIBS) $(INCLUDE) $(FRAMEWORKS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CXX) $(CPPFLAGS) -g -c $< -o $@ $(INCLUDE)

.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

run:
	@cd bin; ./main.out
	
.PHONY: all
all: $(TARGET)
	cp -r $(SCRIPTS_PAHT) $(BIN_PATH)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -rf $(BIN_PATH)/$(SCRIPTS_PAHT)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(DISTCLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)