#include <stdio.h>

#define MAX_PAGES 128

int main(void)
{
    unsigned r[MAX_PAGES] = {0};
    unsigned m[MAX_PAGES] = {0};
    int n;
    char command;

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_PAGES) {
        return 1;
    }

    while (scanf(" %c", &command) == 1) {
        if (command == 'R' || command == 'W') {
            unsigned page;

            if (scanf("%u", &page) != 1 || page >= (unsigned)n) {
                return 1;
            }

            r[page] = 1;
            if (command == 'W') {
                m[page] = 1;
            }
        } else if (command == 'T') {
            for (int page = 0; page < n; ++page) {
                r[page] = 0;
            }
        } else if (command == 'E') {
            unsigned victim = 0;
            unsigned best_class = 4;

            for (unsigned page = 0; page < (unsigned)n; ++page) {
                unsigned page_class = 2 * r[page] + m[page];

                if (page_class < best_class) {
                    best_class = page_class;
                    victim = page;
                }
            }

            printf("EVICT %u class %u\n", victim, best_class);
            return 0;
        } else {
            return 1;
        }
    }

    return 1;
}
