#include "kernel/types.h"
#include "user/user.h"
#include "stddef.h"

// 文件描述符映射
void mapping(int n, int pd[]){
  close(n);
  // 产生一个 n 到 pd[n] 的映射
  dup(pd[n]);
  close(pd[0]), close(pd[1]);
}

// 求素数
void primes()
{
  int previous, next, fd[2];
  // 从管道读取数据
  if (read(0, &previous, sizeof(int))){
    // 第一个一定是素数，直接打印
    printf("prime %d\n", previous);
    // 创建管道
    pipe(fd);
    if (fork() == 0){
      mapping(1, fd);
      while (read(0, &next, sizeof(int)))
        if (next % previous != 0)
          write(1, &next, sizeof(int));
    }
    else{
      wait(NULL);
      mapping(0, fd);
      // 递归
      primes();
    }  
  }  
}

int main(int argc, char *argv[]){
  // 定义描述符
  int fd[2];
  // 创建管道
  pipe(fd);

  if (fork() == 0) { // 子进程
    mapping(1, fd);
    for (int i = 2; i <= 35; ++i)
      write(1, &i, sizeof(int));
  }
  else{
    wait(NULL);
    mapping(0, fd);
    primes();
  }
  exit(0);
}