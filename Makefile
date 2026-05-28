# Compiler and Flags
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS  := 

# Directories
SRC_DIR  := src
INC_DIR  := include
OBJ_DIR  := build
BIN_DIR  := bin
DATA_DIR := data
TEST_DIR := tests

# Target Application Binary
TARGET   := $(BIN_DIR)/exam_scheduler.exe

# Source & Object Discovery for Main Application
SRCS        := $(shell find $(SRC_DIR) -name "*.cpp" 2>/dev/null || dir /B /S src\*.cpp)
MAIN_SRC    := $(SRC_DIR)/main.cpp
CORE_SRCS   := $(filter-out $(MAIN_SRC), $(SRCS))
MAIN_OBJ    := $(OBJ_DIR)/main.o
CORE_OBJS   := $(CORE_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Test Sources Discovery (Finds all test .cpp files inside unit, integration, edge_cases)
TEST_SRCS    := $(shell find $(TEST_DIR) -name "*.cpp" 2>/dev/null || dir /B /S tests\*.cpp)
# This maps each test .cpp to its own independent executable binary path inside bin/tests/
TEST_TARGETS := $(TEST_SRCS:$(TEST_DIR)/%.cpp=$(BIN_DIR)/tests/%.exe)

# Default target
.PHONY: all
all: build_dirs $(TARGET)

# Automatically create all structural required directories
.PHONY: build_dirs
build_dirs:
	@mkdir -p $(OBJ_DIR)/core $(OBJ_DIR)/data_structures $(OBJ_DIR)/utils $(BIN_DIR) frontend
	@mkdir -p $(OBJ_DIR)/tests/unit $(OBJ_DIR)/tests/integration $(OBJ_DIR)/tests/edge_cases
	@mkdir -p $(BIN_DIR)/tests/unit $(BIN_DIR)/tests/integration $(BIN_DIR)/tests/edge_cases

# Link Main App Binary
$(TARGET): $(MAIN_OBJ) $(CORE_OBJS)
	@echo "Linking main application binary..."
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "[SUCCESS] Main application compiled successfully: $@"

# Rule for building each independent test executable
$(BIN_DIR)/tests/%.exe: $(OBJ_DIR)/tests/%.o $(CORE_OBJS)
	@mkdir -p $(dir $@)
	@echo "Linking independent test executable: $@"
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile Application Source Files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile Test Suite Source Files
$(OBJ_DIR)/tests/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run Main Application Target
.PHONY: run
run: all
	@echo "Launching main scheduler binary..."
	./$(TARGET) $(DATA_DIR)/input_sample.json


# AUTOMATED INDEPENDENT TEST RUNNER LOOP
.PHONY: test
test: build_dirs $(TEST_TARGETS)
	@echo "========================================================================="
	@echo ">>> EXECUTING AUTOMATED ISOLATED UNIT, INTEGRATION & EDGE CASE TESTS"
	@echo "========================================================================="
	@for test_exe in $(TEST_TARGETS); do \
		echo "Executing Test Process: $$test_exe"; \
		./$$test_exe; \
		echo "-------------------------------------------------------------------------"; \
	done
	@echo "========================================================================="
    
# Run Valgrind to track main app memory safety leaks
.PHONY: memcheck
memcheck: all
	@echo "========================================================================="
	@echo ">>> RUNNING VALGRIND MEMORY AUDIT TO PROVE 0 BYTES LEAKED"
	@echo "========================================================================="
	@if command -v valgrind > /dev/null; then \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET) $(DATA_DIR)/input_default.json; \
	else \
		echo "[ERROR] Valgrind is not installed in this subsystem framework environment."; \
		echo "Please install it using: sudo apt install valgrind"; \
	fi
	@echo "========================================================================="

# Clean all temporary objects and generated executable layers
.PHONY: clean
clean:
	@echo "Cleaning project directories..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	rm -f frontend/exam_schedule.html
	@echo "[CLEAN] All temporary object modules and target binaries purged successfully."