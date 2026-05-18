CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# İşletim sistemi algılama
ifeq ($(OS),Windows_NT)
    # Windows ayarları
    RM = if exist build rmdir /s /q build
    MKDIR = if not exist build mkdir build
    TARGET = build/app.exe
    CLEAN_CMD = if exist build rmdir /s /q build
else
    # Linux / macOS ayarları
    RM = rm -rf build
    MKDIR = mkdir -p build
    TARGET = build/app
    CLEAN_CMD = rm -rf build
endif

.PHONY: all build run test clean

all: build

build:
	@echo "Derleme basliyor..."
	$(MKDIR)
	$(CXX) $(CXXFLAGS) src/main.cpp -o $(TARGET)

run: build
	@echo "Program calistiriliyor..."
	./$(TARGET) data/input_sample.json

test:
	@echo "Testler henüz hazir degil..."

clean:
	@echo "Temizleniyor..."
	$(CLEAN_CMD)