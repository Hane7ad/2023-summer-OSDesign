#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
char buf[128];

int main(int argc, char* argv[])
{
    int p1[2], p2[2], pid;
    pipe(p1); // 创建管道p1，用于父进程向子进程发送数据
    pipe(p2); // 创建管道p2，用于子进程向父进程发送数据

    if (fork() == 0) // 创建子进程
    {
        close(p2[1]); // 关闭子进程中管道p2的写入端
        close(p1[0]); // 关闭子进程中管道p1的读取端
        pid = getpid(); // 获取子进程的进程ID
        int num = read(p2[0], buf, 1); // 从管道p2的读取端读取1个字节的数据到buf中
        if (num == 1) // 如果成功读取了一个字节的数据
        {
            printf("%d: received ping\n", pid); // 打印接收到ping的消息和子进程的进程ID
            write(p1[1], buf, 1); // 将读取到的数据写入管道p1的写入端，即向父进程发送pong消息
        }
    }
    else // 父进程
    {
        close(p1[1]); // 关闭父进程中管道p1的写入端
        close(p2[0]); // 关闭父进程中管道p2的读取端
        pid = getpid(); // 获取父进程的进程ID
        write(p2[1], buf, 1); // 向管道p2的写入端写入1个字节的数据，即向子进程发送ping消息
        int num = read(p1[0], buf, 1); // 从管道p1的读取端读取1个字节的数据到buf中
        if (num == 1) // 如果成功读取了一个字节的数据
        {
            printf("%d: received pong\n", pid); // 打印接收到pong的消息和父进程的进程ID
        }
    }
    exit(0); // 退出程序
}