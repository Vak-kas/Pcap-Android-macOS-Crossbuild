#include "handler.h"
#include <pcap.h>
#include <stdio.h>
#include "../common.h"
#include <string.h>
#include "../pcapmanager.h"

void handleGetNIC()
{
    std::vector<std::string> nicList = getNICList();
    for (const auto& nic : nicList)
    {
        sendNIC(nic.c_str());
    }
        fflush(stdout);
}

void sendNIC(const char* name)
{
    MsgHeader header;
    header.type = MSG_NIC;
    header.length = strlen(name);

    fwrite(&header, sizeof(header), 1, stdout);
    fwrite(name, header.length, 1, stdout);
    fflush(stdout);
}