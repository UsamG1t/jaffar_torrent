#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
	int i = argc + 1;
	while (argv[i] != NULL) {
		printf("%s\n", argv[i++]);
	}
}
