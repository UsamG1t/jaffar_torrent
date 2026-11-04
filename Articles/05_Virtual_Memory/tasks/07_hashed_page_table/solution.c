#include <stdio.h>

#define MAX_RECORDS 128
#define BUCKET_COUNT 4
#define PAGE_SIZE 256u

struct PageEntry {
    unsigned vp;
    unsigned frame;
    int present;
    int next;
};

int main(void)
{
    struct PageEntry entries[MAX_RECORDS];
    int buckets[BUCKET_COUNT] = {-1, -1, -1, -1};
    int n;
    int q;
    int i;

    if (scanf("%d", &n) != 1 || n < 0 || n > MAX_RECORDS) {
        return 1;
    }

    for (i = 0; i < n; ++i) {
        unsigned vp;
        unsigned frame;
        int present;

        if (scanf("%u %u %d", &vp, &frame, &present) != 3) {
            return 1;
        }

        entries[i].vp = vp;
        entries[i].frame = frame;
        entries[i].present = present;
        entries[i].next = buckets[vp % BUCKET_COUNT];
        buckets[vp % BUCKET_COUNT] = i;
    }

    if (scanf("%d", &q) != 1 || q < 0 || q > MAX_RECORDS) {
        return 1;
    }

    for (i = 0; i < q; ++i) {
        unsigned va;
        unsigned vp;
        unsigned offset;
        int index;
        int found = 0;

        if (scanf("%x", &va) != 1) {
            return 1;
        }

        vp = va / PAGE_SIZE;
        offset = va % PAGE_SIZE;
        index = buckets[vp % BUCKET_COUNT];

        while (index != -1) {
            if (entries[index].vp == vp) {
                found = 1;
                break;
            }
            index = entries[index].next;
        }

        if (!found || !entries[index].present) {
            puts("PAGE_FAULT");
        } else {
            printf("0x%x\n", entries[index].frame * PAGE_SIZE + offset);
        }
    }

    return 0;
}
