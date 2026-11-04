#include <stdio.h>

int main(void)
{
    const int directory[4] = {0, 1, -1, -1};
    const int tables[2][4] = {
        {5, 3, -1, -1},
        {7, -1, -1, -1}
    };
    int q;

    if (scanf("%d", &q) != 1 || q < 0) {
        return 1;
    }

    while (q-- > 0) {
        unsigned int va;
        unsigned int directory_index;
        unsigned int table_index;
        unsigned int offset;
        int table_number;
        int frame;

        if (scanf("%x", &va) != 1) {
            return 1;
        }

        if (va > 0xfffu) {
            puts("PAGE_FAULT");
            continue;
        }

        directory_index = (va >> 10) & 0x3u;
        table_index = (va >> 8) & 0x3u;
        offset = va & 0xffu;
        table_number = directory[directory_index];

        if (table_number < 0) {
            puts("PAGE_FAULT");
            continue;
        }

        frame = tables[table_number][table_index];
        if (frame < 0) {
            puts("PAGE_FAULT");
            continue;
        }

        printf("0x%x\n", (unsigned int)frame * 256u + offset);
    }

    return 0;
}
