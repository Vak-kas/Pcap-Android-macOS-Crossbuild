#include <stdio.h>
#include <unistd.h>

int main()
{
    while (1)
    {
        printf("mjDaemon running...\n");
        fflush(stdout);
        sleep(2);
    }
    return 0;
}