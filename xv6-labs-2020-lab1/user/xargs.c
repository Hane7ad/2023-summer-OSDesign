#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

char args[MAXARG][512];     // 存储命令行参数的二维字符数组
char *pass_args[MAXARG];    // 用于传递给 exec 函数的参数数组
int preargnum, argnum;      // preargnum：命令行参数的数量；argnum：每行输入的参数数量
char ch;                    // 存储读取的字符
char arg_buf[512];          // 临时存储每行输入的参数
int n;                      // 存储 read 函数返回的字节数

// 读取一行输入
int readline()
{
    argnum = preargnum;     // 将命令行参数数量赋值给 argnum
    memset(arg_buf, 0, sizeof(arg_buf));    // 清空参数缓冲区
    for (;;)
    {
        n = read(0, &ch, sizeof(ch));   // 从标准输入读取一个字符
        if (n == 0)
        {
            return 0;   // 如果读取结束，返回 0
        }
        else if (n < 0)
        {
            fprintf(2, "read error\n");   // 读取错误，输出错误信息
            exit(1);
        }
        else
        {
            if (ch == '\n')
            {
                memcpy(args[argnum], arg_buf, strlen(arg_buf) + 1);    // 将参数缓冲区的内容复制到 args 数组中
                argnum++;   // 参数数量加一
                return 1;   // 返回读取成功的状态
            }
            else if (ch == ' ')
            {
                memcpy(args[argnum], arg_buf, strlen(arg_buf) + 1);    // 将参数缓冲区的内容复制到 args 数组中
                argnum++;   // 参数数量加一
                memset(arg_buf, 0, sizeof(arg_buf));   // 清空参数缓冲区
            }
            else
            {
                arg_buf[strlen(arg_buf)] = ch;   // 将读取的字符添加到参数缓冲区
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("usage: xargs [command] [arg1] [arg2] ... [argn]\n");
        exit(0);
    }
    preargnum = argc - 1;   // 获取命令行参数的数量
    for (int i = 0; i < preargnum; i++)
        memcpy(args[i], argv[i + 1], strlen(argv[i + 1]));    // 将命令行参数复制到 args 数组中
    while (readline())   // 循环读取输入的每一行
    {
        if (fork() == 0)
        {
            *args[argnum] = 0;    // 参数数组末尾置空
            int i = 0;
            while (*args[i])
            {
                pass_args[i] = (char *)&args[i];    // 将参数数组中的每个参数地址赋值给 pass_args 数组
                i++;
            }
            *pass_args[argnum] = 0;    // pass_args 数组末尾置空
            exec(pass_args[0], pass_args);   // 执行指定的命令
            printf("exec error\n");    // 如果 exec 函数执行失败，输出错误信息
            exit(0);
        }
        else
        {
            wait((int *)0);   // 父进程等待子进程结束
        }
    }
    exit(0);
}