#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int parseCPUUtilization(const char *line, unsigned long long *idle, unsigned long long *iowait) {
    char cpu_label[4];
    unsigned long long user, nice, system, tmp_idle, tmp_iowait;

    sscanf(line, "%s %llu %llu %llu %llu %llu %llu %llu",
           cpu_label, &user, &nice, &system, &tmp_idle, &tmp_iowait, &user, &user);

    *idle = tmp_idle;
    *iowait = tmp_iowait;

    return 1;
}

int main() {
    FILE *file = fopen("/proc/stat", "r");
    if (file == NULL) {
        perror("Error opening /proc/stat");
        return 1;
    }

    char line[256];
    unsigned long long idle1 = 0, iowait1 = 0, idle2 = 0, iowait2 = 0;
    double avgUtilization = 0.0;

   
    if (fgets(line, sizeof(line), file)) {
        parseCPUUtilization(line, &idle1, &iowait1);
    }

    fclose(file);

    sleep(1); 

    
    file = fopen("/proc/stat", "r");
    if (file == NULL) {
        perror("Error opening /proc/stat");
        return 1;
    }

    if (fgets(line, sizeof(line), file)) {
        parseCPUUtilization(line, &idle2, &iowait2);
    }

    fclose(file);

   
    unsigned long long idle_diff = idle2 - idle1;
    unsigned long long iowait_diff = iowait2 - iowait1;
    unsigned long long total_diff = idle_diff + iowait_diff;

    if (total_diff > 0) {
        avgUtilization = (1.0 - ((double)idle_diff / total_diff)) * 100.0;
    }

    printf("Average CPU Utilization: %.2f%%\n", avgUtilization);

    return 0;
}

