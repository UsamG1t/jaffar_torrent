#include <stdio.h>
#include <stdlib.h>

#define MAX_PARTITIONS 128
#define MAX_PROCESSES 256
#define MAX_ID_LENGTH 31

struct Process {
    char id[MAX_ID_LENGTH + 1];
    unsigned int size;
    int waiting;
};

int main(void)
{
    unsigned int partitions[MAX_PARTITIONS];
    struct Process processes[MAX_PROCESSES];
    int n;
    int m;
    int i;
    int j;

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_PARTITIONS) {
        return EXIT_FAILURE;
    }
    for (i = 0; i < n; ++i) {
        if (scanf("%u", &partitions[i]) != 1) {
            return EXIT_FAILURE;
        }
    }

    if (scanf("%d", &m) != 1 || m < 0 || m > MAX_PROCESSES) {
        return EXIT_FAILURE;
    }
    for (i = 0; i < m; ++i) {
        if (scanf("%31s %u", processes[i].id, &processes[i].size) != 2) {
            return EXIT_FAILURE;
        }
        processes[i].waiting = 1;
    }

    for (i = 0; i < n; ++i) {
        int best = -1;

        for (j = 0; j < m; ++j) {
            if (processes[j].waiting
                && processes[j].size <= partitions[i]
                && (best == -1
                    || processes[j].size > processes[best].size)) {
                best = j;
            }
        }

        if (best == -1) {
            printf("PARTITION %d EMPTY\n", i + 1);
        } else {
            processes[best].waiting = 0;
            printf("PARTITION %d PROCESS %s\n", i + 1,
                   processes[best].id);
        }
    }

    if (m == 0) {
        printf("WAITING -\n");
        return EXIT_SUCCESS;
    }

    printf("WAITING");
    j = 0;
    for (i = 0; i < m; ++i) {
        if (processes[i].waiting) {
            printf(" %s", processes[i].id);
            ++j;
        }
    }
    if (j == 0) {
        printf(" -");
    }
    printf("\n");

    return EXIT_SUCCESS;
}
