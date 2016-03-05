all:
    mkdir -p bin/
    clang --std=c11 -o bin/senbuild src/main.c src/senbuild_funcs.c
