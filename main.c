
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char* read_file(char* path, size_t* file_size) {
	FILE* f = fopen(path, "r");
	if (!f) {
	    printf("Could not open the file.\n");
	    exit(1);
	}
	
	fseek(f, 0, SEEK_END);
	*file_size = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* buf = malloc(*file_size + 1);

	fread(buf, sizeof(char), *file_size, f);
	fclose(f);
    
	return buf;
}

int main(int argc, char** argv) {

	if (argc < 2) {
		printf("please provide a valid assembly file!\n");
		return 1;
	}

	for (int i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}

	size_t file_size;
	char* file = read_file(argv[1], &file_size);

	for (int i = 0; i < file_size; i++) {
		printf("%c", file[i]);
	}
	printf("\n");
	
	return 0;
}
