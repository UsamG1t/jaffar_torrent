#include <stdio.h>

#define MAX_CAPACITY 16
#define MAX_REFERENCES 1000
#define MAX_PAGE 127u

int main(void)
{
    unsigned frames[MAX_CAPACITY];
    int capacity;
    int n;
    int used = 0;
    int next = 0;
    int faults = 0;

    if (scanf("%d %d", &capacity, &n) != 2) {
        return 1;
    }
    if (capacity < 1 || capacity > MAX_CAPACITY ||
        n < 0 || n > MAX_REFERENCES) {
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        unsigned page;
        int found = 0;

        if (scanf("%u", &page) != 1 || page > MAX_PAGE) {
            return 1;
        }

        for (int j = 0; j < used; ++j) {
            if (frames[j] == page) {
                found = 1;
                break;
            }
        }

        if (found) {
            printf("%u HIT\n", page);
            continue;
        }

        ++faults;
        if (used < capacity) {
            frames[used] = page;
            ++used;
            printf("%u FAULT\n", page);
        } else {
            unsigned victim = frames[next];

            frames[next] = page;
            next = (next + 1) % capacity;
            printf("%u FAULT evict %u\n", page, victim);
        }
    }

    printf("FAULTS %d\n", faults);
    return 0;
}
