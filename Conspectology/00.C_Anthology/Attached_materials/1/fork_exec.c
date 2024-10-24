#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(int argc, char** argv)
{
    int count_of_parallel_problems = 0;
    int count_of_sequence_problems = 0;
    
    printf("Parallel\n");
    for (int i = 1; i < argc; i++) {
        pid_t pid_parallel = fork();

        if (pid_parallel < 0) {
            continue;
        }else if (!pid_parallel) {
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        }
    }

    int status_parallel;
    while (wait(&status_parallel) != -1) {
        
        if (WIFEXITED(status_parallel) && !WEXITSTATUS(status_parallel)) {
            count_of_parallel_problems++;
        }
    }

    printf("sequence\n");
    for (int i = 1; i < argc; i++) {
        pid_t pid_sequence = fork();

        if (pid_sequence < 0) {
            _exit(1);
        } else if (!pid_sequence) {
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        } else {
            int status_sequence;
            waitpid(pid_sequence, &status_sequence, 0);

            if (WIFEXITED(status_sequence) && !WEXITSTATUS(status_sequence)) {
                count_of_sequence_problems++;
            }
        }
    }

    printf("%d\n", count_of_parallel_problems);
    printf("%d\n", count_of_sequence_problems);
    return 0;
}
