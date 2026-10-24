#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>


volatile int counter = 0;
pid_t pid1, pid2;
volatile int flag = 1;

void
my_sigusr1(int sig) {
    counter += 5;
    printf("SIGUSR1 %d\n", counter);
    fflush(stdout);
    return;
}

void
my_sigusr2(int sig) {
    counter -= 4;
    printf("SUGUSR2 %d\n", counter);
    fflush(stdout);

    if (counter < 0) {
        signal(SIGUSR1, SIG_DFL);
        signal(SIGUSR2, SIG_DFL);
        kill(pid2, SIGKILL);
        flag = 0;
    }

    return;
}

int
main(int argc, char** argv)
{
    int fd[2];
    if (pipe(fd) < 0) {
        return 1;
    }

    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = my_sigusr1, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGUSR2, &(struct sigaction) { .sa_handler = my_sigusr2, .sa_flags = SA_RESTART}, NULL);

    sigset_t sigmask;
    sigemptyset(&sigmask);
    
    sigaddset(&sigmask, SIGUSR1);
    sigaddset(&sigmask, SIGUSR2);

    sigset_t old;

    sigprocmask(SIG_BLOCK, &sigmask, &old);
    
    if ((pid1 = fork()) < 0) {
        _exit(1);
    } else if (!pid1) {
        read(fd[0], &pid2, sizeof(pid2));
        close(fd[0]);
        close(fd[1]);

        sigset_t empty;
        sigemptyset(&empty);

        while(flag) {
            sigsuspend(&empty);
        }

        return 0;
    }

    if ((pid2 = fork()) < 0) {
        _exit(1);
    } else if (!pid2) {
        int usr_sig;
        while(scanf("%d", &usr_sig) == 1) {
            if (usr_sig & 1) {
                kill(pid1, SIGUSR1);
            } else {
                kill(pid1, SIGUSR2);
            }
        }

        return 0;
    }

    write(fd[1], &pid2, sizeof(pid2));
    close(fd[1]);
    close(fd[0]);

    int status;
    while(wait(&status) != -1);

    return 0;
}
