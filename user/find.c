#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

// find 函数
void find(char *dir, char *file)
{   
    // buf数组：缓冲区
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(dir, 0)) < 0)
    {
        // 报错，无法打开此路径
        fprintf(2, "find: cannot open %s\n", dir);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        // 出错
        fprintf(2, "find: cannot stat %s\n", dir);
        close(fd);
        return;
    }

    if (st.type != T_DIR)
    {
        // 类型不是目录错误
        fprintf(2, "find: %s is not a directory\n", dir);
        close(fd);
        return;
    }

    // 路径过长
    if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        fprintf(2, "find: directory too long\n");
        close(fd);
        return;
    }

    strcpy(buf, dir);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if(de.inum == 0)
            continue;
        if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;
        memmove(p, de.name, DIRSIZ);

        if(stat(buf, &st) < 0)
        {
            // 出错
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }
        // 递归查找
        if (st.type == T_DIR)
        {
            find(buf, file);
        }
        else if (st.type == T_FILE && !strcmp(de.name, file))
        {
            // 打印缓冲区存放的路径
            printf("%s\n", buf);
        } 
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "usage: find dirName fileName\n");
        // 异常退出
        exit(1);
    }
    // 调用 find 函数查找指定目录下的文件
    find(argv[1], argv[2]);
    exit(0);
}
