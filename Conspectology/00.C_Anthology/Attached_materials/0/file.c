#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>

enum
{
    NUM = 26
};

int main(void) {
    char name[PATH_MAX];
    fgets(name, PATH_MAX, stdin); // Да, stdin это тоже как будто FILE*

    if (strlen(name) > 0 && name[strlen(name) - 1] == '\n') {
        name[strlen(name) - 1] = '\0';
    }
    
    FILE* f1 = fopen(name, "r"); // Открыли файл на чтение

    if (f1 == NULL) {
        return 1;
    }

    unsigned long count[NUM] = {};
    int a;

    int i = 0;
    while ((a = fgetc(f1)) != EOF) { // достаём по символу из файла
        if(isalpha(a)) {
            count[tolower(a) - 'a']++;
        }
    }

    for (int i = 0; i < NUM; i++) {
        printf("\'%c\' - %lu\n", 'a' + i, count[i]);
    }

    return 0;
}
