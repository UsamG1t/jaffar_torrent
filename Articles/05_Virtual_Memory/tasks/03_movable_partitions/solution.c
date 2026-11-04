#include <stdio.h>
#include <string.h>

#define MAX_BLOCKS 1000
#define MAX_ID_LENGTH 31

typedef struct {
    char id[MAX_ID_LENGTH + 1];
    unsigned size;
} Block;

int main(void)
{
    Block blocks[MAX_BLOCKS + 1];
    unsigned memory_size;
    unsigned used = 0;
    unsigned total_free = 0;
    unsigned new_size;
    char new_id[MAX_ID_LENGTH + 1];
    int n;
    int first_fit = -1;

    if (scanf("%u%d", &memory_size, &n) != 2 ||
        memory_size == 0 || n < 1 || n > MAX_BLOCKS) {
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        if (scanf("%31s%u", blocks[i].id, &blocks[i].size) != 2 ||
            blocks[i].size == 0 || blocks[i].size > memory_size - used) {
            return 1;
        }
        used += blocks[i].size;
        if (strcmp(blocks[i].id, "HOLE") == 0) {
            total_free += blocks[i].size;
        }
    }

    if (used != memory_size ||
        scanf("%31s%u", new_id, &new_size) != 2 ||
        new_size == 0 || strcmp(new_id, "HOLE") == 0) {
        return 1;
    }

    if (new_size > total_free) {
        puts("NOT_ENOUGH_MEMORY");
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        if (strcmp(blocks[i].id, "HOLE") == 0 &&
            blocks[i].size >= new_size) {
            first_fit = i;
            break;
        }
    }

    if (first_fit >= 0) {
        unsigned remainder = blocks[first_fit].size - new_size;

        strcpy(blocks[first_fit].id, new_id);
        blocks[first_fit].size = new_size;

        if (remainder > 0) {
            for (int i = n; i > first_fit + 1; --i) {
                blocks[i] = blocks[i - 1];
            }
            strcpy(blocks[first_fit + 1].id, "HOLE");
            blocks[first_fit + 1].size = remainder;
            ++n;
        }
    } else {
        int compacted = 0;
        unsigned remainder = total_free - new_size;

        for (int i = 0; i < n; ++i) {
            if (strcmp(blocks[i].id, "HOLE") != 0) {
                if (compacted != i) {
                    blocks[compacted] = blocks[i];
                }
                ++compacted;
            }
        }

        strcpy(blocks[compacted].id, new_id);
        blocks[compacted].size = new_size;
        n = compacted + 1;

        if (remainder > 0) {
            strcpy(blocks[n].id, "HOLE");
            blocks[n].size = remainder;
            ++n;
        }
    }

    for (int i = 0; i < n; ++i) {
        printf("%s %u\n", blocks[i].id, blocks[i].size);
    }

    return 0;
}
