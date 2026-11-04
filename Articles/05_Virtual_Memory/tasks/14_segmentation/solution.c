#include <stdio.h>

#define SEGMENT_COUNT 4U

struct Segment {
    unsigned base;
    unsigned size;
};

int main(void)
{
    struct Segment segments[SEGMENT_COUNT];
    unsigned q;
    unsigned i;

    for (i = 0; i < SEGMENT_COUNT; ++i) {
        if (scanf("%u %u", &segments[i].base, &segments[i].size) != 2) {
            return 1;
        }
    }

    if (scanf("%u", &q) != 1) {
        return 1;
    }

    for (i = 0; i < q; ++i) {
        unsigned segment;
        unsigned offset;

        if (scanf("%u %u", &segment, &offset) != 2) {
            return 1;
        }

        if (segment >= SEGMENT_COUNT || offset >= segments[segment].size) {
            puts("SEGMENT_FAULT");
        } else {
            printf("%u\n", segments[segment].base + offset);
        }
    }

    return 0;
}
