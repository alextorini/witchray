SRC_DIR := src
BUILD_DIR := build
RES_DIR := res
INCLUDES := -I. -Isrc
CPPSTD := -std=c++20

.DEFAULT_GOAL := help

.PHONY: help build build-linux build-mingw build-web run-web clean

help:
	@echo "Available commands:"
	@echo "  make build           - Build for current platform (Linux)"
	@echo "  make build-linux     - Build Linux native executable (clang++)"
	@echo "  make build-mingw     - Build Windows executable (cross-compile)"
	@echo "  make build-web       - Build WebAssembly version (Emscripten)"
	@echo "  make run-web         - Start web server for WebAssembly build"
	@echo "  make clean           - Remove all build artifacts"
	@echo "  make all             - Build all platforms"
	@echo ""

build: build-linux

build-linux:
	@echo "[Building for Linux...]"
	mkdir -p $(BUILD_DIR)
	clang++ -g -O0 $(CPPSTD) $$(find ./$(SRC_DIR) -name "*.cpp") \
		$(INCLUDES) \
		-o $(BUILD_DIR)/witchray \
		-lraylib -lm -lpthread -ldl -lrt -lX11
	@echo "[✓ Linux build complete: $(BUILD_DIR)/witchray]"

build-mingw:
	@echo "[Building for Windows (MinGW)...]"
	mkdir -p $(BUILD_DIR)
	x86_64-w64-mingw32-g++ \
		-g -O0 $(CPPSTD) \
		$(INCLUDES) \
		$(SRC_DIR)/*.cpp \
		lib/debug/libraylib.a \
		-static-libgcc -static-libstdc++ -static -lwinpthread \
		-lopengl32 \
		-lgdi32 \
		-lwinmm \
		-lshell32 \
		-luser32 \
		-o $(BUILD_DIR)/mingw_witchray.exe
	@echo "[✓ Windows build complete: $(BUILD_DIR)/mingw_witchray.exe]"

build-web:
	@echo "[Building for Web (WebAssembly)...]"
	mkdir -p $(BUILD_DIR)/web
	em++ $$(find ./$(SRC_DIR) -name "*.cpp") \
		$(CPPSTD) \
		$(INCLUDES) \
		~/raylib/raylib/src/libraylib.web.a \
		-o $(BUILD_DIR)/web/index.html \
		-DPLATFORM_WEB \
		-s USE_GLFW=3 \
		-s ASYNCIFY \
		-sALLOW_MEMORY_GROWTH=0 -sINITIAL_MEMORY=128MB \
		--shell-file ~/raylib/raylib/src/minshell.html \
		--preload-file $(RES_DIR)
	@echo "[✓ Web build complete: open http://localhost:8000/build/web in browser after 'make run-web']"

run-web:
	@echo "[Starting web server on http://localhost:8000]"
	@echo "[Press Ctrl+C to stop]"
	cd $(BUILD_DIR)/web && python -m http.server 8000

all: build-linux build-mingw build-web
	@echo "[✓ All builds complete!]"

clean:
	@echo "[Cleaning build artifacts...]"
	rm -rf $(BUILD_DIR)
	@echo "[✓ Clean complete]"
