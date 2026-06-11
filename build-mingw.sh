mkdir -p build

x86_64-w64-mingw32-gcc \
    src/*.c \
    lib/debug/libraylib.a \
    -lopengl32 \
    -lgdi32 \
    -lwinmm \
    -lshell32 \
    -luser32 \
    -o build/mingw_witchray.exe
