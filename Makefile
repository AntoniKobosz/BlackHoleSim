CXX = g++
CXXFLAGS = -Iinclude -Llib -O3
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Llib

SRC_DIR = src
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TARGET = $(BUILD_DIR)/SkyBox.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean
