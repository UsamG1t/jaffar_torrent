#include <stdio.h>

int main(void)
{
    unsigned os_size;
    unsigned memory_size;
    unsigned n;

    if (scanf("%u %u %u", &os_size, &memory_size, &n) != 3) {
        return 1;
    }

    for (unsigned i = 0; i < n; ++i) {
        unsigned address;

        if (scanf("%u", &address) != 1) {
            return 1;
        }

        if (address < os_size) {
            puts("OS");
        } else if (address < memory_size) {
            puts("USER");
        } else {
            puts("OUT_OF_RANGE");
        }
    }

    return 0;
}
