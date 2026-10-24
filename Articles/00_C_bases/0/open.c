#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

enum
{
	BUF_SIZE = 1024
};

int
main(int argc, char** argv)
{
	char buf[BUF_SIZE];
	char* p;
	int fd_1, fd_2;
	
	fd_1 = open(argv[1], O_RDONLY);
	fd_2 = creat(argv[2], strtol(argv[3], &p, 0));

	int count;

	while((count = read(fd_1, buf, BUF_SIZE)) > 0) {
		write(fd_2, buf, count);
	}

	close(fd_1);
	close(fd_2);

	return 0;
}
