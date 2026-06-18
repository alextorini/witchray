mkdir -p build/web

emcc $(find ./src -name "*.c") \
    -I. \
    -I~/raylib/raylib/src \
    ~/raylib/raylib/src/libraylib.web.a \
    -o build/web/index.html \
    -DPLATFORM_WEB \
    -s USE_GLFW=3 \
    -s ASYNCIFY \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s TOTAL_MEMORY=134217728 \
    --shell-file ~/raylib/raylib/src/minshell.html \
    --preload-file res
