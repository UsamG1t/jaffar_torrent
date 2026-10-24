#include <unistd.h>

int
main(int argc, char** argv)
{
	if (argc == 1) {
		return 0;
	}

	execvp(argv[1], argv + 1);
	_exit(1);
}