#include <stdio.h>

#define MAX_FRAMES 128
#define PAGE_SIZE 256u

typedef struct {
    unsigned pid;
    unsigned vp;
    int valid;
} InvertedEntry;

int main(void)
{
    InvertedEntry table[MAX_FRAMES];
    int frames;
    int q;
    int i;

    if (scanf("%d", &frames) != 1 || frames < 1 || frames > MAX_FRAMES) {
        return 1;
    }

    for (i = 0; i < frames; ++i) {
        if (scanf("%u %u %d", &table[i].pid, &table[i].vp,
                  &table[i].valid) != 3 ||
            (table[i].valid != 0 && table[i].valid != 1)) {
            return 1;
        }
    }

    if (scanf("%d", &q) != 1 || q < 0) {
        return 1;
    }

    for (i = 0; i < q; ++i) {
        unsigned pid;
        unsigned va;
        unsigned vp;
        unsigned offset;
        int frame;
        int found = 0;

        if (scanf("%u %x", &pid, &va) != 2) {
            return 1;
        }

        vp = va / PAGE_SIZE;
        offset = va % PAGE_SIZE;

        for (frame = 0; frame < frames; ++frame) {
            if (table[frame].valid && table[frame].pid == pid &&
                table[frame].vp == vp) {
                found = 1;
                break;
            }
        }

        if (found) {
            unsigned pa = (unsigned)frame * PAGE_SIZE + offset;
            printf("0x%x\n", pa);
        } else {
            printf("PAGE_FAULT\n");
        }
    }

    return 0;
}
