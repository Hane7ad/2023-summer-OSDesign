#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if(argc != 2){
        fprintf(2,"usage: sleep <time>\n");
        exit(1);
    }
    int second = atoi(argv[1]);
    printf("is sleeping...\n");
    sleep(second);
    exit(0);
}