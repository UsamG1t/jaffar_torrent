#include <stdio.h>

#define MAX_CAPACITY 16

static void clear_frame(unsigned char matrix[MAX_CAPACITY][MAX_CAPACITY],
                        int capacity, int frame)
{
    int j;

    for (j = 0; j < capacity; ++j) {
        matrix[frame][j] = 0;
        matrix[j][frame] = 0;
    }
}

static void access_frame(unsigned char matrix[MAX_CAPACITY][MAX_CAPACITY],
                         int capacity, int frame)
{
    int j;

    for (j = 0; j < capacity; ++j) {
        matrix[frame][j] = 1;
    }
    for (j = 0; j < capacity; ++j) {
        matrix[j][frame] = 0;
    }
}

static unsigned row_value(unsigned char matrix[MAX_CAPACITY][MAX_CAPACITY],
                          int capacity, int frame)
{
    unsigned value = 0;
    int j;

    for (j = 0; j < capacity; ++j) {
        value |= (unsigned)matrix[frame][j] << j;
    }
    return value;
}

int main(void)
{
    unsigned char matrix[MAX_CAPACITY][MAX_CAPACITY] = {{0}};
    unsigned pages[MAX_CAPACITY];
    unsigned char used[MAX_CAPACITY] = {0};
    int capacity;
    int n;
    int faults = 0;
    int i;

    if (scanf("%d %d", &capacity, &n) != 2) {
        return 1;
    }
    if (capacity < 1 || capacity > MAX_CAPACITY || n < 0 || n > 1000) {
        return 1;
    }

    for (i = 0; i < n; ++i) {
        unsigned page;
        int frame = -1;
        int j;

        if (scanf("%u", &page) != 1) {
            return 1;
        }

        for (j = 0; j < capacity; ++j) {
            if (used[j] && pages[j] == page) {
                frame = j;
                break;
            }
        }

        if (frame >= 0) {
            printf("%u HIT\n", page);
            access_frame(matrix, capacity, frame);
            continue;
        }

        ++faults;
        for (j = 0; j < capacity; ++j) {
            if (!used[j]) {
                frame = j;
                break;
            }
        }

        if (frame >= 0) {
            printf("%u FAULT\n", page);
        } else {
            unsigned best_value = 0;

            frame = -1;
            for (j = 0; j < capacity; ++j) {
                unsigned value = row_value(matrix, capacity, j);

                if (frame < 0 || value < best_value) {
                    frame = j;
                    best_value = value;
                }
            }
            printf("%u FAULT evict %u\n", page, pages[frame]);
            clear_frame(matrix, capacity, frame);
        }

        pages[frame] = page;
        used[frame] = 1;
        access_frame(matrix, capacity, frame);
    }

    printf("FAULTS %d\n", faults);
    return 0;
}
