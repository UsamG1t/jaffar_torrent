#include <stdio.h>

#define MAX_PAGES 128
#define MAX_TLB 16
#define PAGE_SIZE 256u

struct Page {
    unsigned vp;
    unsigned frame;
    unsigned present;
};

struct TlbEntry {
    unsigned vp;
    unsigned frame;
    unsigned long load_time;
    int valid;
};

int main(void) {
    struct Page pages[MAX_PAGES];
    struct TlbEntry tlb[MAX_TLB] = {{0, 0, 0, 0}};
    unsigned capacity, n, q;
    unsigned long load_time = 0;

    if (scanf("%u %u", &capacity, &n) != 2 ||
        capacity < 1 || capacity > MAX_TLB || n > MAX_PAGES) {
        return 1;
    }

    for (unsigned i = 0; i < n; ++i) {
        if (scanf("%u %u %u", &pages[i].vp, &pages[i].frame,
                  &pages[i].present) != 3) {
            return 1;
        }
    }

    if (scanf("%u", &q) != 1) {
        return 1;
    }

    for (unsigned query = 0; query < q; ++query) {
        unsigned va, vp, offset, frame = 0;
        unsigned page_index = n;
        int hit = 0;

        if (scanf("%x", &va) != 1) {
            return 1;
        }

        vp = va / PAGE_SIZE;
        offset = va % PAGE_SIZE;

        for (unsigned i = 0; i < capacity; ++i) {
            if (tlb[i].valid && tlb[i].vp == vp) {
                frame = tlb[i].frame;
                hit = 1;
                break;
            }
        }

        if (hit) {
            printf("HIT 0x%x\n", frame * PAGE_SIZE + offset);
            continue;
        }

        for (unsigned i = 0; i < n; ++i) {
            if (pages[i].vp == vp) {
                page_index = i;
                break;
            }
        }

        if (page_index == n || pages[page_index].present == 0) {
            printf("PAGE_FAULT\n");
            continue;
        }

        unsigned slot = 0;
        while (slot < capacity && tlb[slot].valid) {
            ++slot;
        }

        if (slot == capacity) {
            slot = 0;
            for (unsigned i = 1; i < capacity; ++i) {
                if (tlb[i].load_time < tlb[slot].load_time) {
                    slot = i;
                }
            }
        }

        ++load_time;
        tlb[slot].vp = pages[page_index].vp;
        tlb[slot].frame = pages[page_index].frame;
        tlb[slot].load_time = load_time;
        tlb[slot].valid = 1;

        printf("MISS 0x%x\n",
               pages[page_index].frame * PAGE_SIZE + offset);
    }

    return 0;
}
