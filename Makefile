CXX := g++
TARGET := libflute3wrapper.a
CXXFLAGS := -std=c++11 -O3 -static -static-libstdc++ -static-libgcc
WARNINGFLAGS := -Wall -Wextra
FLUTE_DIR := flute3
FLUTE_BUILD_DIR := $(FLUTE_DIR)/build
FLUTE_LIB := $(FLUTE_BUILD_DIR)/libflute.a
SRC_DIRS := .
SRCS := $(wildcard $(SRC_DIRS:=/*.cpp))
OBJS := $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)
FLUTEWRAPPER_SRCS := Flute3Wrapper.cpp
FLUTEWRAPPER_OBJS := $(FLUTEWRAPPER_SRCS:.cpp=.o)
TEST_SRCS := test.cpp
TEST_OBJS := $(TEST_SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(FLUTE_LIB) $(FLUTEWRAPPER_OBJS)
	ar -rcs temp.a $(FLUTEWRAPPER_OBJS)
	printf "create $@\naddlib temp.a\naddlib $(FLUTE_LIB)\nsave\nend" | ar -M
	rm temp.a

$(FLUTE_LIB):
	mkdir -p $(FLUTE_BUILD_DIR)
	cd $(FLUTE_BUILD_DIR) && cmake ..
	$(MAKE) -C $(FLUTE_BUILD_DIR)

$(FLUTEWRAPPER_OBJS): $(FLUTEWRAPPER_SRCS)
	$(CXX) $(CXXFLAGS) $(WARNINGFLAGS) -isystem $(FLUTE_DIR) -MMD -c $< -o $@

test: $(TEST_OBJS) $(TARGET)
	$(CXX) -o $@ $^ -lstdc++

$(TEST_OBJS): $(TEST_SRCS)
	$(CXX) $(CXXFLAGS) $(WARNINGFLAGS) -MMD -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJS) $(DEPS)
	if [ -d "$(FLUTE_BUILD_DIR)" ]; then $(MAKE) -C $(FLUTE_BUILD_DIR) clean; fi

.PHONY: all clean test
-include $(DEPS)