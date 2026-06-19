CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude
LDFLAGS  :=

SRC_DIR  := src
SRCS     := $(SRC_DIR)/Asset.cpp \
            $(SRC_DIR)/Market.cpp \
            $(SRC_DIR)/ConsoleUI.cpp \
            $(SRC_DIR)/FileManager.cpp \
            $(SRC_DIR)/GameManager.cpp \
            main.cpp

OBJS     := $(SRCS:.cpp=.o)
TARGET   := terminal

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
