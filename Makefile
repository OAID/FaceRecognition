
BUILD_DIR=./build

include ./config.mk

include detect/Makefile

DETECT_OBJS:=$(addprefix $(BUILD_DIR)/detect/, $(detect-obj-y))


include align/Makefile
ALIGN_OBJS:=$(addprefix $(BUILD_DIR)/align/, $(align-obj-y))

include lib/Makefile
LIB_OBJS=$(addprefix $(BUILD_DIR)/lib/, $(lib-obj-y))

include extract/Makefile
EXTRACT_OBJS=$(addprefix $(BUILD_DIR)/extract/, $(extract-obj-y))

include verify/Makefile
VERIFY_OBJS=$(addprefix $(BUILD_DIR)/verify/, $(verify-obj-y))

include store/Makefile
STORE_OBJS=$(addprefix $(BUILD_DIR)/store/, $(store-obj-y))

include test/Makefile
TEST_OBJS=$(addprefix $(BUILD_DIR)/test/, $(test-obj-y))

include bin/Makefile
BIN_OBJS=$(addprefix $(BUILD_DIR)/bin/, $(bin-obj-y))



COMM_CFLAGS= -Wall -g   $(OPT)  -I./include

CXXFLAGS+=$(COMM_CFLAGS)  
CFLAGS+=$(COMM_CFLAGS)

MODEL_DIR=$(realpath ./models)
CXXFLAGS+=-DMODEL_DIR=\"$(MODEL_DIR)\"

CC=gcc  -std=gnu99
CXX=g++ -std=c++11
LD=g++


COMMON_OBJS=$(DETECT_OBJS) $(ALIGN_OBJS) $(LIB_OBJS) $(EXTRACT_OBJS) $(VERIFY_OBJS) $(STORE_OBJS)

BIN_OBJS+=$(TEST_OBJS)
BIN_EXES=$(BIN_OBJS:.o=)


all : $(BIN_EXES) 


$(BIN_EXES): $(COMMON_OBJS) 

$(BIN_EXES):$(BUILD_DIR)/%:$(BUILD_DIR)/%.o


clean:
	rm -f $(BIN_EXES) $(BIN_OBJS) $(COMMON_OBJS) 

.PHONY : all clean

$(BUILD_DIR)/%.o:%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS)  -c $< -o $@ 

$(BUILD_DIR)/%.o : %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS)  -c $< -o $@ 

$(BUILD_DIR)/%: $(BUILD_DIR)/%.o
	@mkdir -p $(@D)
	$(LD) $(LDFLAGS) $< $(COMMON_OBJS) -o $@ $(LIBS) 

$(BUILD_DIR)/test/%: $(BUILD_DIR)/test/%.o 
	@mkdir -p $(@D)
	$(LD) $(LDFLAGS) $< $(COMMON_OBJS) -o $@ $(LIBS) $(TEST_LIBS)

