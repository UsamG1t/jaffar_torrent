#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

enum
{
    N_PROC = 10
};

struct msg_buf {
    long mtype;
    long mval;
};

int
main(int argc, char** argv)
{
    if (argc < 2) {
        return 1;
    }

    char* p;
    unsigned step = strtol(argv[1], &p, 10);
 

    pid_t pid;
    unsigned son_num, val = 0;

    int msg = msgget(IPC_PRIVATE, IPC_CREAT | 0666);

    for (int i = 0; i < N_PROC; i++) {
        son_num = i;

        if ((pid = fork()) < 0) {
            return i + 1;
        } else if(!pid) {
            printf("%u -> %ld\n", son_num + 1, getpid());
            fflush(stdout);

            struct msg_buf in_buf = {son_num + 1, getpid()}, out_buf;
            msgsnd(msg, &in_buf, sizeof(in_buf) - sizeof(in_buf.mtype), 0);
            msgrcv(msg, &out_buf, sizeof(out_buf) - sizeof(out_buf.mtype), (son_num + step) % N_PROC + 1, 0);

            printf("%u + %u -> %ld\n", son_num + 1, step, out_buf.mval);
            fflush(stdout);

            _exit(0);
        }
    }

    while(wait(NULL) > 0);
    msgctl(msg, IPC_RMID, 0);

    return 0;
}
