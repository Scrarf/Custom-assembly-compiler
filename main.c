#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

// section struct for stuff like "start 0x08:"

typedef struct token {
	char* str;
	int size;
} token;

token tokens[(1 << 16) - 1];
int token_ptr = 0;

typedef struct section_table {
	char* str;
	uint32_t addr;
} section_table;

section_table sec_tb[(1 << 16) - 1];
uint32_t sec_tb_ptr;

uint32_t code[(1 << 16) - 1];
uint32_t code_ptr;


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



int interpret() {
	int i = 0;
	//while (i < tokens_ptr) {
	
		if (strncmp(tokens[i].str, "section", tokens[i].size) == 0) {
			sec_tb[sec_tb_ptr].str = tokens[i+1].str;
			
			if (tokens[i+2].str[0] == '0' && tokens[i+2].str[1] == 'x') {
				int tmp_addr = 0;
				int addr = 0;
				for (int j = 2; j < tokens[i+2].size-2; j++) {
					if (tokens[i+2].str[j] >= '0' && tokens[i+2].str[j] <= '9') {
						tmp_addr = (tokens[i+2].str[j] - '0');
					}
					else if (tokens[i+2].str[j] >= 'A' && tokens[i+2].str[j] <= 'F') {
						tmp_addr = tokens[i+2].str[j] - 'A' + 10;
					}
					else {
						printf("Invalid address.\n");
						return 1;	
					}
					//addr += tmp_addr * pow(16, tokens[i+2].size-3 - (j-2) - 1);
					printf("addr: %d\n", addr);
					addr += tmp_addr << ((tokens[i+2].size-4 - (j-2) - 1) * 4);
				}
				sec_tb[sec_tb_ptr].addr = addr;
				sec_tb_ptr++;
			}
			else {
				printf("Invalid address.\n");
				return 1;
			}
		
		}
		printf("addr: %d\n", sec_tb[0].addr);	
	//}
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
		printf("exiting due to tokenization falure.\n");
		return 1;
	}

	if (interpret() != 0) {
		printf("exiting due to interpretation falure.\n");
		return 1;
	}
	
	return 0;
}


