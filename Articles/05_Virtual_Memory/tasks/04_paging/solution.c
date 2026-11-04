#include <stdio.h>
#include <stddef.h>

#define MAX_ENTRIES 128
#define PAGE_SIZE 256U

typedef struct {
    unsigned vp;
    unsigned frame;
    int present;
} PageEntry;

int main(void)
{
    PageEntry entries[MAX_ENTRIES];
    unsigned n;
    unsigned q;
    size_t i;

    if (scanf("%u", &n) != 1 || n > MAX_ENTRIES) {
        return 1;
    }

    for (i = 0; i < n; ++i) {
        if (scanf("%u%u%d", &entries[i].vp, &entries[i].frame,
                  &entries[i].present) != 3) {
            return 1;
        }
        if (entries[i].present != 0 && entries[i].present != 1) {
            return 1;
        }
    }

    if (scanf("%u", &q) != 1) {
        return 1;
    }

    for (i = 0; i < q; ++i) {
        unsigned va;
        unsigned vp;
        unsigned offset;
        size_t j;
        int found = 0;

        if (scanf("%x", &va) != 1) {
            return 1;
        }

        vp = va / PAGE_SIZE;
        offset = va % PAGE_SIZE;

        for (j = 0; j < n; ++j) {
            if (entries[j].vp == vp) {
                found = 1;
                if (entries[j].present != 0) {
                    unsigned pa = entries[j].frame * PAGE_SIZE + offset;
                    printf("0x%x\n", pa);
                }
                break;
            }
        }

        if (!found || entries[j].present == 0) {
            puts("PAGE_FAULT");
        }
    }

    return 0;
}
