#include <stdio.h>

#define MAX_PAGES 128u

int main(void)
{
    unsigned n;
    unsigned counter[MAX_PAGES] = {0};
    unsigned char referenced[MAX_PAGES] = {0};
    char command;

    if (scanf("%u", &n) != 1 || n == 0u || n > MAX_PAGES) {
        return 1;
    }

    while (scanf(" %c", &command) == 1) {
        if (command == 'R') {
            unsigned p;

            if (scanf("%u", &p) != 1 || p >= n) {
                return 1;
            }
            referenced[p] = 1;
        } else if (command == 'T') {
            unsigned p;

            for (p = 0u; p < n; ++p) {
                counter[p] += referenced[p];
                referenced[p] = 0;
            }
        } else if (command == 'E') {
            unsigned p;
            unsigned victim = 0u;

            for (p = 1u; p < n; ++p) {
                if (counter[p] < counter[victim]) {
                    victim = p;
                }
            }
            printf("EVICT %u counter %u\n", victim, counter[victim]);
            return 0;
        } else {
            return 1;
        }
    }

    return 1;
}
