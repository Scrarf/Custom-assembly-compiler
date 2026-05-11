
all:
	gcc -g main.c -o custom-assembly-compiler
	./custom-assembly-compiler assembly.asm
