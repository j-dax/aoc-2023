CXX := g++
CXXFLAGS := -std=c++20 # -Wall -Wextra -O2
LDFLAGS :=

SRC_DIRS := $(wildcard day*)
SRCS := $(wildcard $(addsuffix /*.cpp,$(SRC_DIRS))) main.cpp
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

TARGET := aoc

.PHONY: all clean

all: $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) main.o -o $@

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)
