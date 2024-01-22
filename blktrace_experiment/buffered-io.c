#include <stdio.h>
#include <stdlib.h> 

#define FILE_PATH "test"
#define BUFFER_SIZE 4096  // 4KB
#define READ_END 16777216  // 16MB

int main(void) {
	FILE* fp = fopen(FILE_PATH, "r");
	if (fp == NULL) {
		perror("Error opening file for reading");
		exit(EXIT_FAILURE);
	}

	char buf[BUFFER_SIZE];
	size_t bytesRead = 0;

	while (bytesRead < READ_END) {
		size_t result = fread(buf, 1, BUFFER_SIZE, fp);
		if (result == 0) {
			break;  // End of file or error
		}

		bytesRead += result;
	}

	fclose(fp);
	
	return 0;
}
