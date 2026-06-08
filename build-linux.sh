mkdir -p build

gcc -g -O0 $(find ./src -name "*.c") \
    -I. \
    -o build/witchray \
    -lraylib -lm -lpthread -ldl -lrt -lX11
