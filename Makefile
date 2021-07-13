CXX := gcc
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

all: $(TARGET)

$(TARGET): $(FLUTE_DIR) $(OBJS)
	ar -rcs $(TARGET) $(OBJS)

FORCE: ;

$(FLUTE_DIR): FORCE
	mkdir -p $(FLUTE_BUILD_DIR)
	cd $(FLUTE_BUILD_DIR) && cmake ..
	$(MAKE) -C $(FLUTE_BUILD_DIR)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(WARNINGFLAGS) -isystem $(FLUTE_DIR) -l $(FLUTE_LIB) -MMD -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJS) $(DEPS)
	rm -rf $(FLUTE_BUILD_DIR)

.PHONY: all clean
-include $(DEPS)