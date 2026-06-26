mkdir -p build

gcc -g -O0 $(find ./src -name "*.c") \
    -I. \
    -o build/witchray \
    -lraylib -lm -lpthread -ldl -lrt -lX11

g++ -g -O0 $(find ./src -name "*.c") \
    -I. \
    -o build/witchray++ \
    -lraylib -lm -lpthread -ldl -lrt -lX11

x86_64-w64-mingw32-gcc \
    -g -O0 \
    src/*.c \
    lib/debug/libraylib.a \
    -lopengl32 \
    -lgdi32 \
    -lwinmm \
    -lshell32 \
    -luser32 \
    -o build/mingw_witchray.exe
