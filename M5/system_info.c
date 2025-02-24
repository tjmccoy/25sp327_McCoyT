#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define COLUMN_FORMAT "%-29s"

void print_hostname() {
    FILE *fp = fopen("/proc/sys/kernel/hostname", "r");
    if (fp == NULL) {
        perror("Error opening /proc/sys/kernel/hostname");
        return;
    }

    char line[256];
    if (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\n")] = 0;
        printf(COLUMN_FORMAT "%s\n", "Current Host Name:", line);
    } else {
        perror("Error reading hostname");
    }

    fclose(fp);
}

void print_cpuinfo() {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL) {
        perror("Error opening /proc/cpuinfo");
        return;
    }

    char line[256];
    char cpu_type[256] = "n/a";
    char cpu_model[256] = "n/a";
    int cpu_cores = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "vendor_id", 9) == 0) {
            sscanf(line, "vendor_id : %s", cpu_type);
        } else if (strncmp(line, "model name", 10) == 0) {
            sscanf(line, "model name : %[^\n]", cpu_model); // %[^\n] == read everything until EOL
        } else if (strncmp(line, "cpu cores", 9) == 0) {
            sscanf(line, "cpu cores : %d", &cpu_cores);
        }
    }

    fclose(fp);

    printf(COLUMN_FORMAT "%s\n", "CPU Type:", cpu_type);
    printf(COLUMN_FORMAT "%s\n", "CPU Model:", cpu_model);
    printf(COLUMN_FORMAT "%d\n", "CPU Cores:", cpu_cores);
}

void print_version() {
    FILE *fp = fopen("/proc/version", "r");
    if (fp == NULL) {
        perror("Error opening /proc/version");
        return;
    }

    char kernel_version[128];
    char line[256];

    if (fgets(line, sizeof(line), fp) != NULL) {
        sscanf(line, "Linux version %s", kernel_version);
        printf(COLUMN_FORMAT "%s\n", "Kernel Version", kernel_version);
    } else {
        perror("Error reading version");
    }

    fclose(fp);
}

void print_meminfo() {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        perror("Error opening /proc/meminfo");
        return;
    }

    char line[256];
    long total_mem = 0;
    long available_mem = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "MemTotal:", 9) == 0) {
            sscanf(line, "MemTotal: %ld kB", &total_mem);
        } else if (strncmp(line, "MemAvailable:", 13) == 0) {
            sscanf(line, "MemAvailable: %ld kB", &available_mem);
        }
    }

    fclose(fp);

    printf(COLUMN_FORMAT "%ld kB\n", "Memory Configured:", total_mem);
    printf(COLUMN_FORMAT "%ld kB\n", "Memory Available:", available_mem);
}

void print_uptime() {
    FILE *fp = fopen("/proc/uptime", "r");
    if (fp == NULL) {
        perror("Error opening /proc/uptime");
        return;
    }

    double uptime_seconds;
    char line[256];
    
    if (fgets(line, sizeof(line), fp) != NULL) {
        sscanf(line, "%lf", &uptime_seconds);

        int days = (int)(uptime_seconds / 86400);
        int hours = (int)((uptime_seconds - days * 86400) / 3600);
        int minutes = (int)((uptime_seconds - days * 86400 - hours * 3600) / 60);
        int seconds = (int)(uptime_seconds - days * 86400 - hours * 3600 - minutes * 60);

        printf(COLUMN_FORMAT "%d:%02d:%02d:%02d\n", "Uptime:", days, hours, minutes, seconds);
    } else {
        perror("Error reading uptime");
    }

    fclose(fp);
}

void print_stat() {
    FILE *fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        perror("Error opening /proc/stat");
        return;
    }

    char line[256];
    unsigned long user_time = 0;
    unsigned long system_time = 0;
    unsigned long idle_time = 0;

    if (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "cpu ", 4) == 0){
            sscanf(line, "cpu %lu %*u %lu %lu", &user_time, &system_time, &idle_time);
        }
    }

    fclose(fp);

    printf(COLUMN_FORMAT "%lu ms\n", "Time Spent in User Mode:", user_time * 10);
    printf(COLUMN_FORMAT "%lu ms\n", "Time Spent in System Mode:", system_time * 10);
    printf(COLUMN_FORMAT "%lu ms\n", "Time Spent in Idle Mode:", idle_time * 10);
}

int main() {
    printf("----------------------------------------------------------------------------\n");
    printf("                            System Information\n");
    printf("----------------------------------------------------------------------------\n");

    print_hostname();
    print_cpuinfo();
    print_version();
    print_meminfo();
    print_uptime();
    print_stat();

    printf("----------------------------------------------------------------------------\n");
    return 0;
}