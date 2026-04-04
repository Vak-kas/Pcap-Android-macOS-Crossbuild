#include "handler.h"
#include <pcap.h>
#include <stdio.h>
#include "common.h"
#include <string.h>

void handleGetNIC()
{
    pcap_if_t *alldevs;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        //TODO : Protocol 핸들링
        return;
    }

    for (pcap_if_t *d = alldevs; d != NULL; d = d->next)
    {
        if (d->name)
            sendNIC(d->name);
    }

    pcap_freealldevs(alldevs);
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