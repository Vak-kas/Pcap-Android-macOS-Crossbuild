#include <stdio.h>
#include <unistd.h>
#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include "handler.h"

char cmd[256];

int main()
{
    while (fgets(cmd, sizeof(cmd), stdin)) 
    {
        if (strncmp(cmd, "GET_NIC", 7) == 0) {
            handleGetNIC();
        }
        usleep(100000);
    }
    return 0;
}