all:
		mkdir -p bin/
		clang -o bin/senbuild src/main.c src/senbuild_funcs.c
