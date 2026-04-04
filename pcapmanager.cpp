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
