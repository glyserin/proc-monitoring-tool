#include <stdio.h>
#include <stdlib.h>

#define DISK_NAME "mmcblk0"

int main() {
	FILE* diskstats_file;
	char line[256];
	char disk_name[32];
	unsigned long sectors_read = 0, sectors_written = 0, sectors_discarded = 0;
	
	diskstats_file = fopen("/proc/diskstats", "r");
	if (diskstats_file == NULL) {
		perror("Failed to open /proc/diskstats");
		return EXIT_FAILURE;
	}

	while (fgets(line, sizeof(line), diskstats_file)) {
		unsigned long temp_sectors_read, temp_sectors_written, temp_sectors_discarded;

		sscanf(line, "%*d %*d %s %*d %*d %lu %*d %*d %*d %lu %*d %*d %*d %*d %lu",
               disk_name, &temp_sectors_read, &temp_sectors_written, &temp_sectors_discarded);

		if (strcmp(disk_name, DISK_NAME) == 0) {
			sectors_read = temp_sectors_read;
			sectors_written = temp_sectors_written;
			sectors_discarded = temp_sectors_discarded;
			break;
		}
	}

	fclose(diskstats_file);

	printf("Sectors read (KB): %lu\n", sectors_read / 2);
	printf("Sectors written (KB): %lu\n", sectors_written / 2);
	printf("Sectors discarded (KB): %lu\n", sectors_discarded / 2);

	return EXIT_SUCCESS;
}
