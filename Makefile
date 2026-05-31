CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# OS Detection & Shell Override
ifeq ($(OS),Windows_NT)
    SHELL := cmd.exe
    TARGET = build/app.exe
    TEST_UNIT = build/test_unit.exe
    TEST_INT = build/test_integration.exe
    TEST_EDGE = build/test_edge.exe
    MKDIR_CMD = if not exist build mkdir build
    CLEAN_CMD = if exist build rmdir /s /q build
    RUN_CMD = $(TARGET)
    RUN_UNIT = $(TEST_UNIT)
    RUN_INT = $(TEST_INT)
    RUN_EDGE = $(TEST_EDGE)
else
    # Linux / macOS settings
    TARGET = build/app
    TEST_UNIT = build/test_unit
    TEST_INT = build/test_integration
    TEST_EDGE = build/test_edge
    MKDIR_CMD = mkdir -p build
    CLEAN_CMD = rm -rf build
    RUN_CMD = ./$(TARGET)
    RUN_UNIT = ./$(TEST_UNIT)
    RUN_INT = ./$(TEST_INT)
    RUN_EDGE = ./$(TEST_EDGE)
endif

.PHONY: all build run test memcheck docs clean

all: build

build:
	@echo "Compiling main application..."
	$(MKDIR_CMD)
	$(CXX) $(CXXFLAGS) src/main.cpp -o $(TARGET)

run: build
	@echo "Running program..."
	$(RUN_CMD) data/input_sample.json

test:
	@echo "Compiling modular test suites..."
	$(MKDIR_CMD)
	$(CXX) $(CXXFLAGS) tests/unit/test_data_structures.cpp -o $(TEST_UNIT)
	$(CXX) $(CXXFLAGS) tests/integration/test_scheduler_pipeline.cpp -o $(TEST_INT)
	$(CXX) $(CXXFLAGS) tests/edge_cases/test_room_saturation.cpp -o $(TEST_EDGE)
	@echo "========================================="
	@echo "        EXECUTING TEST SUITES            "
	@echo "========================================="
	@echo ">>> 1. UNIT TESTS"
	$(RUN_UNIT)
	@echo ">>> 2. INTEGRATION TESTS"
	$(RUN_INT)
	@echo ">>> 3. EDGE CASE TESTS"
	$(RUN_EDGE)

memcheck: test
	@echo "Checking for memory leaks with Valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(RUN_INT)

docs:
	@echo "Generating Doxygen documentation..."
	doxygen docs/Doxyfile

clean:
	@echo "Cleaning up..."
	$(CLEAN_CMD)