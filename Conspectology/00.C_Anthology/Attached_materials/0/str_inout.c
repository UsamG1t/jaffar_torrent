#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    MAX_LEN = 256
};

int
main(void)
{
    char string[MAX_LEN];
    char result[MAX_LEN];
    int i = 1;

    while(fgets(string, MAX_LEN, stdin)) {
        char* str = string;
        double a, sum = 0;
        int offset;

        while(sscanf(str, "%lf%n", &a, &offset) == 1) { // %n - считает количество байт от начального указателя после чтения числа
            sum += a;
            str += offset;
        }
        snprintf(result, MAX_LEN, "Sum of numbers in the %d-%s string is %lf\n", i, 
            (i == 1) ? "st" : ((i == 2) ? "nd" : ((i == 3) ? "rd" : "th")), sum);
        printf("%s", result);

        i++;
    }


    return 0;
}
