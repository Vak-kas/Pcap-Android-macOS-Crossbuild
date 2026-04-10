#include <string>
#include <vector>
#include <pcap.h>
#include "common.h"

std::vector<std::string> getNICList();
pcap_t* initPcap(const std::string& device);
void closePcap(pcap_t* pcap);
