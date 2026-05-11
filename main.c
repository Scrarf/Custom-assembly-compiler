#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// section struct for stuff like "start 0x08:"

typedef struct token {
	char* str;
	int size;
} token;
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

int tokenize(char* text, size_t size) {
	int line_number = 1;
	int allocated = 0;

	token tokens[(1 << 16) - 1];
	int token_ptr = 0;
	
	for (int i = 0; i < size; i++) {
		
		if ((text[i] >= 35) && (text[i] <= 122)) {
			if (!allocated) {
				tokens[token_ptr].str = malloc(64);
				tokens[token_ptr].size = 0;
				allocated = 1;
			}
			tokens[token_ptr].str[tokens[token_ptr].size] = text[i];
			tokens[token_ptr].size++;
			
			if (tokens[token_ptr].size > 63) {
				printf("Token size greater than 63 found.");
			}

		} else if (isspace(text[i])) {
			if (text[i] == '\n') {
				line_number++;
			}
			if (allocated) {
				tokens[token_ptr].str[tokens[token_ptr].size] = '\0';
				tokens[token_ptr].size++;
				allocated = 0;
				token_ptr++;
			}
			
		} else {
			printf("Invalid charachter found at line %d\n", line_number);
			printf("charachter: %d\n", text[i]);
			return 1;
		}
		
		
			
	}
	printf("Tokens:\n");
	for (int i = 0; i < token_ptr - 1; i++) {
		printf("token %d: %s\n", i, tokens[i]);
	}
	return 0;
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
	char* text = read_file(argv[1], &file_size);

	for (int i = 0; i < file_size; i++) {
		printf("%c", text[i]);
	}
	printf("\n");

	 if (tokenize(text, file_size) != 0) {
		printf("exiting due to falure.\n");
		return 1;
	 }	
	
	return 0;
}
