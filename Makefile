CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# OS Detection
ifeq ($(OS),Windows_NT)
    # Windows settings
    MKDIR = if not exist build mkdir build
    TARGET = build/app.exe
    TEST_TARGET = build/test_app.exe
    CLEAN_CMD = if exist build rmdir /s /q build
else
    # Linux / macOS settings
    MKDIR = mkdir -p build
    TARGET = build/app
    TEST_TARGET = build/test_app
    CLEAN_CMD = rm -rf build
endif

.PHONY: all build run test memcheck docs clean

all: build

build:
	@echo "Compiling..."
	$(MKDIR)
	$(CXX) $(CXXFLAGS) src/main.cpp -o $(TARGET)

run: build
	@echo "Running program..."
	./$(TARGET) data/input_sample.json

# 1. TEST TARGET: Compiles and runs test files
test:
	@echo "Compiling and running tests..."
	$(MKDIR)
	$(CXX) $(CXXFLAGS) tests/test_main.cpp -o $(TEST_TARGET)
	./$(TEST_TARGET)

# 2. MEMCHECK TARGET: Checks for memory leaks using Valgrind
memcheck: test
	@echo "Checking for memory leaks with Valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_TARGET)

# 3. DOCS TARGET: Generates Doxygen documentation
docs:
	@echo "Generating Doxygen documentation..."
	doxygen docs/Doxyfile

clean:
	@echo "Cleaning up..."
	$(CLEAN_CMD)