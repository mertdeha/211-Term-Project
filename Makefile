CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# OS Detection & Shell Override
ifeq ($(OS),Windows_NT)
    SHELL := cmd.exe
    TARGET = build/app.exe
    TEST_TARGET = build/test_app.exe
    MKDIR_CMD = if not exist build mkdir build
    CLEAN_CMD = if exist build rmdir /s /q build
    RUN_CMD = $(TARGET)
    TEST_RUN_CMD = $(TEST_TARGET)
else
    # Linux / macOS settings
    TARGET = build/app
    TEST_TARGET = build/test_app
    MKDIR_CMD = mkdir -p build
    CLEAN_CMD = rm -rf build
    RUN_CMD = ./$(TARGET)
    TEST_RUN_CMD = ./$(TEST_TARGET)
endif

.PHONY: all build run test memcheck docs clean

all: build

build:
	@echo "Compiling..."
	$(MKDIR_CMD)
	$(CXX) $(CXXFLAGS) src/main.cpp -o $(TARGET)

run: build
	@echo "Running program..."
	$(RUN_CMD) data/input_sample.json

test:
	@echo "Compiling and running tests..."
	$(MKDIR_CMD)
	$(CXX) $(CXXFLAGS) tests/test_main.cpp -o $(TEST_TARGET)
	$(TEST_RUN_CMD)

memcheck: test
	@echo "Checking for memory leaks with Valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(TEST_RUN_CMD)

docs:
	@echo "Generating Doxygen documentation..."
	doxygen docs/Doxyfile

clean:
	@echo "Cleaning up..."
	$(CLEAN_CMD)