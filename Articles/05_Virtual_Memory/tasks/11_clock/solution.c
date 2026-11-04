#include <stdio.h>

#define MAX_FRAMES 16

typedef struct {
    unsigned page;
    int occupied;
    int referenced;
} Frame;

int main(void)
{
    unsigned capacity;
    unsigned n;
    Frame frames[MAX_FRAMES] = {{0, 0, 0}};
    unsigned hand = 0;
    unsigned faults = 0;
    unsigned i;

    if (scanf("%u %u", &capacity, &n) != 2 ||
        capacity < 1 || capacity > MAX_FRAMES || n > 1000) {
        return 1;
    }

    for (i = 0; i < n; ++i) {
        unsigned page;
        unsigned j;
        unsigned free_index = capacity;
        int hit = 0;

        if (scanf("%u", &page) != 1 || page > 127) {
            return 1;
        }

        for (j = 0; j < capacity; ++j) {
            if (frames[j].occupied && frames[j].page == page) {
                frames[j].referenced = 1;
                hit = 1;
                break;
            }
        }

        if (hit) {
            printf("%u HIT\n", page);
            continue;
        }

        ++faults;
        for (j = 0; j < capacity; ++j) {
            if (!frames[j].occupied) {
                free_index = j;
                break;
            }
        }

        if (free_index < capacity) {
            frames[free_index].page = page;
            frames[free_index].occupied = 1;
            frames[free_index].referenced = 1;
            printf("%u FAULT\n", page);
        } else {
            unsigned victim;

            while (frames[hand].referenced) {
                frames[hand].referenced = 0;
                hand = (hand + 1) % capacity;
            }

            victim = frames[hand].page;
            frames[hand].page = page;
            frames[hand].referenced = 1;
            hand = (hand + 1) % capacity;
            printf("%u FAULT evict %u\n", page, victim);
        }
    }

    printf("FAULTS %u\n", faults);
    return 0;
}
