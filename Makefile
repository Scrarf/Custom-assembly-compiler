
all:
	gcc main.c -o custom-assembly-compiler
	./custom-assembly-compiler assembly.asm
