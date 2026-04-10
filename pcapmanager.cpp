#include "pcapmanager.h"
#include <string>
#include <vector>
#include <pcap.h>

// nic_service.cpp
std::vector<std::string> getNICList()
{
    std::vector<std::string> result;

    pcap_if_t* alldevs;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&alldevs, errbuf) == -1)
        return result;

    for (pcap_if_t* d = alldevs; d; d = d->next)
    {
        if (d->name)
            result.push_back(d->name);
    }

    pcap_freealldevs(alldevs);
    return result;
}

pcap_t* initPcap(const std::string& device)
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap = pcap_open_live(device.c_str(), BUFSIZ, 1, 1000, errbuf);

    if (pcap == NULL) {
        printf("pcap_open_live error: %s\n", errbuf);
        return nullptr;
    }

    return pcap;
}

void closePcap(pcap_t* pcap)
{
    if (pcap)
        pcap_close(pcap);
}

