#include "pcapmanager.h"
#include <pcap.h>

QList<NICInfo> PcapManager::getNICList()
{
    QList<NICInfo> list;

    pcap_if_t *alldevs;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        return list;
    }

    for (pcap_if_t *d = alldevs; d != nullptr; d = d->next) {
        NICInfo nic;

        nic.name = d->name;
        nic.description = d->description
                              ? QString(d->description)
                              : "No description";

        list.append(nic);
    }

    pcap_freealldevs(alldevs);
    return list;
}
