CC := gcc
CXX := g++

#SHELL_PWD = $(shell pwd)
SHELL_PWD = .

INC_DIR := $(SHELL_PWD)
SRC_DIR := $(SHELL_PWD)
ASN_SRC := $(SHELL_PWD)
OBJ_DIR := $(SHELL_PWD)
BIN_DIR := $(SHELL_PWD)
LIB_DIR := $(SHELL_PWD)

VPATH = $(SRC_DIR)

#目标文件名
TARGET  := $(BIN_DIR)/a.out

INCPATHS := \
    -I${INC_DIR} \
	-I$(ASN_SRC) \

LIBDIRS :=  \
    -L$(LIB_DIR) \
#要链接pthread库  protobuf使用了系统多线程库
LIBS := \
    -lpthread \
	-lm \
	-lprotobuf \
	-pthread

CFLAGS := -Wall $(INCPATHS) -g -Wno-unused-but-set-variable -Wno-unused-variable -g #-DASN_EMIT_DEBUG=1
CXXFLAGS := -Wall $(INCPATHS) -g -std=c++11 -Wno-unused-but-set-variable -Wno-unused-variable
LDFLAGS := $(LIBDIRS) $(LIBS)

# CPP files
SOURCES_CPP := $(foreach dir,$(subst :, ,$(VPATH)),$(wildcard $(dir)/*.cpp))
SOURCES_CPP += $(foreach dir,$(subst :, ,$(VPATH)),$(wildcard $(dir)/*.cc))

# C files
SOURCES_C := $(foreach dir,$(subst :, ,$(VPATH)),$(wildcard $(dir)/*.c))

# all source files
SOURCES_FILES := $(SOURCES_CPP) $(SOURCES_C)
#排除某些文件不编译
#SOURCES_FILES := $(filter-out ${OTH_SRC}/rdbMapper.c ${OTH_SRC}/Sync.cc, $(SOURCES_FILES))

# object files
OBJS := $(addsuffix .o,$(basename $(SOURCES_FILES)))
OBJS_FILES := $(foreach n,$(notdir $(OBJS)),$(OBJ_DIR)/$(n))


all:$(TARGET) $(OBJS_FILES)

$(info $(SOURCES_FILES))
$(info $(TARGET))
$(info $(OBJS_FILES))

$(TARGET) : $(OBJS_FILES)
	@mkdir -p  $(LIB_DIR) $(BIN_DIR) 2> /dev/null
	$(CXX)  -o $(TARGET) $(OBJS_FILES) $(LDFLAGS)

${OBJ_DIR}/%.o:%.c
	@mkdir -p $(OBJ_DIR) 2> /dev/null
	$(CC) -c $(CFLAGS) $< -o $@

${OBJ_DIR}/%.o:%.cpp
	@mkdir -p $(OBJ_DIR) 2> /dev/null
	$(CXX) -c $(CXXFLAGS) $< -o $@

${OBJ_DIR}/%.o:%.cc
	@mkdir -p $(OBJ_DIR) 2> /dev/null
	$(CXX) -c $(CXXFLAGS) $< -o $@


.PHONY : clean

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
