#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/mman.h> 
#include <sys/uio.h>  // For the iovec structure

#define FILE_PATH "./test"
#define BUFFER_SIZE 4096  // 4KB
#define READ_END 16777216  // 16MB
#define PAGE_SIZE 4096  // 4KB

int main(void) {
	int fd = open(FILE_PATH, O_RDONLY | O_DIRECT);
	if (fd == -1) {
		perror("Error opening file for reading");
		exit(EXIT_FAILURE);
	}
	
	// Allocate two aligned memory buffers
	void *buf1, *buf2;
	if (posix_memalign(&buf1, PAGE_SIZE, BUFFER_SIZE) != 0 || posix_memalign(&buf2, PAGE_SIZE, BUFFER_SIZE) != 0) {
		close(fd);
		perror("Failed to allocate aligned memory");
		exit (EXIT_FAILURE);
	}

	// Define the iovec structure	
	struct iovec iov[2];	
	iov[0].iov_base = buf1;	
	iov[0].iov_len = BUFFER_SIZE;	
	iov[1].iov_base = buf2;	
	iov[1].iov_len = BUFFER_SIZE;	
	
	off_t offset = 0;
	while (offset < READ_END) {
		// Use readv instead of pread
		ssize_t bytesRead = preadv(fd, iov, 2, offset);
		if (bytesRead <= 0) {
			break;  // End of file or error
		}
	
		offset += bytesRead;
	}

	free(buf1);
	free(buf2);
	close(fd);

	return 0;
}

