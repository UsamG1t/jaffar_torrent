#include <stdio.h>

struct descriptor {
    unsigned base;
    unsigned limit;
    int present;
};

int main(void)
{
    static const struct descriptor gdt[2] = {
        {0, 0, 0},
        {0x00000000u, 0x0000ffffu, 1}
    };
    static const struct descriptor ldt[2] = {
        {0, 0, 0},
        {0x00010000u, 0x0000ffffu, 1}
    };
    int page_table[17];
    int q;
    int i;

    for (i = 0; i < 17; ++i) {
        page_table[i] = -1;
    }
    page_table[0] = 5;
    page_table[1] = 7;
    page_table[16] = 9;

    if (scanf("%d", &q) != 1 || q < 0) {
        return 1;
    }

    for (i = 0; i < q; ++i) {
        unsigned selector;
        unsigned offset;
        unsigned index;
        unsigned ti;
        unsigned linear;
        unsigned dir;
        unsigned table;
        unsigned offset_in_page;
        const struct descriptor *desc;
        int frame;

        if (scanf("%x %x", &selector, &offset) != 2) {
            return 1;
        }

        index = selector >> 3;
        ti = (selector >> 2) & 1u;
        if (index >= 2) {
            puts("SEGMENT_FAULT");
            continue;
        }

        desc = ti == 0 ? &gdt[index] : &ldt[index];
        if (!desc->present || offset > desc->limit) {
            puts("SEGMENT_FAULT");
            continue;
        }

        linear = desc->base + offset;
        dir = (linear >> 22) & 0x3ffu;
        table = (linear >> 12) & 0x3ffu;
        offset_in_page = linear & 0xfffu;

        if (dir != 0) {
            puts("PAGE_FAULT");
            continue;
        }
        if (table >= 17) {
            puts("PAGE_FAULT");
            continue;
        }

        frame = page_table[table];
        if (frame < 0) {
            puts("PAGE_FAULT");
            continue;
        }

        printf("0x%x\n", ((unsigned)frame << 12) | offset_in_page);
    }

    return 0;
}
