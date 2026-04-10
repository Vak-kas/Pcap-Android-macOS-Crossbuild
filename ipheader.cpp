#include "ipheader.h"

void IpHeader::setVersionHlen(uint8_t version, uint8_t len)
{
    version_hlen = (version << 4) | (len & 0x0F);
}

void IpHeader::setSrcDstIp(Ip src_ip_, Ip dst_ip_)
{
    src_ip = src_ip_;
    dst_ip = dst_ip_;
}

void IpHeader::setTos(uint8_t tos_)
{
    tos = tos_;
}

void IpHeader::setTotalLength(uint16_t total_length_)
{
    total_length = total_length_;
}

void IpHeader::setIdentifier(uint16_t identifier_)
{
    identifier = identifier_;
}

void IpHeader::setFlag(uint16_t flag_)
{
    flag = flag_;
}

void IpHeader::setTTL(uint8_t ttl_)
{
    ttl = ttl_;
}

void IpHeader::setProtocol(uint8_t protocol_)
{
    protocol = protocol_;
}
