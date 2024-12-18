#include "kernel/types.h"
#include "user/user.h"
#include "stddef.h"

int main(int argc, char *argv[])
{
    int pfd1[2], pfd2[2];
    int pid;
    char c = 'p';

    // 创建两个管道，每个方向一个
    pipe(pfd1);
    pipe(pfd2);

    // 创建子进程
    pid = fork();
    if (pid == 0) { // 子进程
        read(pfd1[0], &c, 1);
        printf("%d: received ping\n", getpid(), c); // 打印接收到的ping消息
        write(pfd2[1], &c, 1);
    } else { // 父进程
        write(pfd1[1], &c, 1);
        wait(NULL);
        read(pfd2[0], &c, 1);
        printf("%d: received pong\n", getpid(), c); // 打印接收到的pong消息
    }

    exit(0);
}
