#include <dirent.h>
#include <stdio.h>

int main(void) {
    struct dirent *de;
  
    DIR *dr = opendir("/proc");

    if (dr == NULL) {
        printf("Could not open /proc directory" );
        return 0;
    }
  

    while ((de = readdir(dr)) != NULL)
            printf("%s\n", de->d_name);

    closedir(dr);
    return 0;
}
