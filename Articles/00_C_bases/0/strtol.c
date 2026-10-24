#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char** argv) {
	long lval;
	int sum = 0;
	char *eptr;

	for (int i = 1; i < argc; i++) {
		eptr = NULL;
		errno = 0;

		lval = strtol(argv[i], &eptr, 23);
		if (errno || *eptr || eptr == argv[i] || (int) lval != lval) {
			_exit(1);
		}

		sum += lval;
	}

	printf("%d\n", sum);

	return 0;
}