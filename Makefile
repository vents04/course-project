# Cross-platform Makefile for Windows, macOS, and Linux
# Works with GNU Make, MinGW Make, and MSYS2 Make
# Supports cross-compilation from macOS/Linux to Windows

# Detect OS
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    EXE_EXT := .exe
    RM := del /F /Q
    RMDIR := rmdir /S /Q
    MKDIR := mkdir
    RUN_PREFIX :=
else
    DETECTED_OS := $(shell uname -s 2>/dev/null || echo "Unknown")
    EXE_EXT :=
    RM := rm -f
    RMDIR := rm -rf
    MKDIR := mkdir -p
    RUN_PREFIX := ./
endif

# Cross-compilation toolchain for Windows
MINGW_CXX := x86_64-w64-mingw32-g++
MINGW_AVAILABLE := $(shell which $(MINGW_CXX) 2>/dev/null)

# Detect compiler (g++ or clang++)
ifeq ($(DETECTED_OS),Windows)
    # On Windows, try where command first, then fallback
    CXX := $(shell where g++ 2>nul || where clang++ 2>nul || echo "g++")
else
    # On Unix-like systems, try which or command -v
    CXX := $(shell which g++ 2>/dev/null || which clang++ 2>/dev/null || command -v g++ 2>/dev/null || command -v clang++ 2>/dev/null || echo "g++")
endif

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -Iinclude

# Project structure
TARGET = optical_system$(EXE_EXT)
TARGET_WINDOWS = optical_system.exe
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BUILD_DIR_WIN = build_win
DOCS_DIR = docs

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
OBJECTS_WIN = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR_WIN)/%.o,$(SOURCES))

# Default target (native build)
all: $(TARGET)

# Link executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
ifeq ($(DETECTED_OS),Windows)
	@echo Build successful! Run with: $(TARGET)
else
	@echo "✓ Build successful! Run with: $(RUN_PREFIX)$(TARGET)"
endif

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(wildcard $(INCLUDE_DIR)/*.h) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory
$(BUILD_DIR):
ifeq ($(DETECTED_OS),Windows)
	@if not exist $(BUILD_DIR) $(MKDIR) $(BUILD_DIR)
else
	@$(MKDIR) $(BUILD_DIR)
endif

# Windows cross-compilation targets
windows: check-mingw $(TARGET_WINDOWS)

check-mingw:
ifeq ($(MINGW_AVAILABLE),)
	@echo "Error: MinGW cross-compiler not found!"
	@echo "On macOS, install with: brew install mingw-w64"
	@echo "On Linux (Ubuntu/Debian), install with: sudo apt-get install mingw-w64"
	@exit 1
else
	@echo "Using MinGW cross-compiler: $(MINGW_CXX)"
endif

$(TARGET_WINDOWS): $(OBJECTS_WIN)
	$(MINGW_CXX) $(CXXFLAGS) -o $(TARGET_WINDOWS) $(OBJECTS_WIN) -static-libgcc -static-libstdc++
	@echo "✓ Windows build successful! File: $(TARGET_WINDOWS)"

$(BUILD_DIR_WIN)/%.o: $(SRC_DIR)/%.cpp $(wildcard $(INCLUDE_DIR)/*.h) | $(BUILD_DIR_WIN)
	$(MINGW_CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR_WIN):
	@$(MKDIR) $(BUILD_DIR_WIN)

# Build for both native and Windows
all-platforms: all windows

# Clean build artifacts
clean:
ifeq ($(DETECTED_OS),Windows)
	@if exist $(BUILD_DIR)\*.o $(RM) $(BUILD_DIR)\*.o 2>nul
	@if exist $(BUILD_DIR_WIN)\*.o $(RM) $(BUILD_DIR_WIN)\*.o 2>nul
	@if exist $(TARGET) $(RM) $(TARGET) 2>nul
	@if exist $(TARGET_WINDOWS) $(RM) $(TARGET_WINDOWS) 2>nul
	@if exist suppliers.dat $(RM) suppliers.dat 2>nul
	@if exist orders.dat $(RM) orders.dat 2>nul
	@echo Cleaned build artifacts
else
	@$(RM) $(BUILD_DIR)/*.o $(TARGET) 2>/dev/null || true
	@$(RM) $(BUILD_DIR_WIN)/*.o $(TARGET_WINDOWS) 2>/dev/null || true
	@$(RM) suppliers.dat orders.dat 2>/dev/null || true
	@echo "✓ Cleaned build artifacts"
endif

# Run the executable
run: $(TARGET)
	$(RUN_PREFIX)$(TARGET)

# Rebuild from scratch
rebuild: clean all

# Show help
help:
	@echo "Available targets:"
	@echo "  make              - Compile the project for native platform"
	@echo "  make windows      - Cross-compile for Windows (.exe)"
	@echo "  make all-platforms- Build for both native and Windows"
	@echo "  make run          - Compile and run"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make rebuild      - Clean and recompile"
	@echo "  make help         - Show this help message"

.PHONY: all windows all-platforms check-mingw clean clean-data clean-all run rebuild help
