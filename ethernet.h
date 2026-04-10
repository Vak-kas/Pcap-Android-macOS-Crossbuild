#pragma once
#pragma pack(push, 1)
#include <stdint.h>
#include <arpa/inet.h>
#include "mac.h"

struct EthernetHeader
{
private:
    Mac dst_mac;
    Mac src_mac;
    uint16_t eth_type;

public:
    Mac const& dmac() const { return dst_mac; };
    Mac const& smac() const { return src_mac; };
    uint16_t type() const { return ntohs(eth_type); };
};

enum EtherType : uint16_t
{
    ipv4 = 0x0800,
    arp = 0x0806,
    ipv6 = 0x06dd
};
