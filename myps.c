#include <dirent.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(void) {
	struct dirent *de;
  
	DIR *dr = opendir("/proc");
	FILE *file;

	char path[200];

	char pid[256];
	char tty[100];
	char cmd[400];

	if (dr == NULL) {
		printf("Could not open /proc directory" );
		return 0;
	}
  
	printf("%-10s%-10s%s\n", "PID", "TTY", "CMD");	
	while ((de = readdir(dr)) != NULL) {
		if (de->d_type != DT_DIR)
			continue;

		strcpy(pid, de->d_name);
		if (atoi(pid) <= 0)
			continue;

		strcpy(path, "/proc/");
		strcat(path, de->d_name);
		strcat(path, "/stat");

		file = fopen(path, "r");
		if (file == NULL) {
			perror("Error opening file");
			return 1;
		}
		
		if(fscanf(file, "%*d %s %*s %*s %*s %*s %s", cmd, tty) == 2) {
			printf("%-10s", pid);
			printf("%-10s", tty);
			printf("%s\n", cmd);
		}
		fclose(file);
	}

	closedir(dr);
	return 0;
}

