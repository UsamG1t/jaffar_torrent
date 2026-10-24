#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

int
main(int argc, char** argv)
{
    printf("Open_format: full clear - 0; append - 1\n");
    int fd_1;
    scanf("%d", &fd_1);


    if (!fd_1) {
        fd_1 = open(argv[1], O_RDWR | O_TRUNC | O_CREAT, 0666);
    } else {
        fd_1 = open(argv[1], O_RDWR);
    }
    printf("%d\n", fd_1);
    if (fd_1 == -1) {
        printf("Create file (*.bin) before\n");
        return 1;
    }

    int n;
    printf("count\n");
    scanf("%d", &n);

    int shift;
    printf("shift\n");
    scanf("%d", &shift);
    lseek(fd_1, shift, 0);

    int type;
    printf("Type: Int (1) / Double (2) / Long Long (3)  / Char (4) / Short (5)\n"); 
    scanf("%d", &type);


    for (int i = 0; i < n; i++) {
        
        if (type == 1) {
            int a;
            scanf("%d", &a);
            write(fd_1, &a, sizeof(a));
        }

        if (type == 2) {
            double a;
            scanf("%lf", &a);
            write(fd_1, &a, sizeof(a));
        }

        if (type == 3) {
            long long a;
            scanf("%lld", &a);
            write(fd_1, &a, sizeof(a));
        }

        if (type == 4) {
            int a;
            scanf("%d", &a);

            char c = 'A' - 'A' + a;
            write(fd_1, &c, sizeof(c));
            // lseek(fd_1, -3, 1);
        }

        if (type == 5) {
            short a;
            scanf("%hd", &a);

            write(fd_1, &a, sizeof(a));
            // lseek(fd_1, -3, 1);
        }

    }

    close(fd_1);

    return 0;
}
