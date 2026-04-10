#include <stdio.h>
#include <unistd.h>
#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include "handler.h"
#include "../common.h"
#include "../pcapmanager.h"

char cmd[256];
pid_t g_child_pid = -1;

int main()
{
    while (1)
    {
        // 1. 명령 처리
        if (fgets(cmd, sizeof(cmd), stdin))
        {
            if (strncmp(cmd, "GET_NIC", 7) == 0)
            {
                handleGetNIC();
            }
            else if (strncmp(cmd, "START_CAPTURE", 13) == 0)
            {
                char* nic = cmd + 14;
                nic[strcspn(nic, "\n")] = 0; // 개행 제거
                handleStartCapture(nic);
            }
            else if (strncmp(cmd, "STOP_CAPTURE", 12) == 0)
            {
                handleStopCapture();
            }
            else if (strncmp(cmd, "EXIT", 4) == 0)
            {
                handleStopCapture();
                exit(0);
            }
        }
            usleep(10000); // 10ms
    }
}