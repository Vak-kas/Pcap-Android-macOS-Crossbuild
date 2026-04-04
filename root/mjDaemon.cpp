#include <stdio.h>
#include <unistd.h>
#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include "handler.h"

char cmd[256];

int main()
{
    while (1)
    {
        if(fgets(cmd, sizeof(cmd), stdin) != NULL)
        {
            if(strcmp(cmd, "GET_NIC") == 0)
            {
                handleGetNIC();
            }
        }
    }
    return 0;
}