#pragma once
#include <stdint.h>
#include "ip.h"
#include <arpa/inet.h>

#pragma pack(push, 1)
struct IpHeader
{
private:
    uint8_t version_hlen;
    uint8_t tos;
    uint16_t total_length;
    uint16_t identifier;
    uint16_t flag;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t header_checksum;
    Ip src_ip;
    Ip dst_ip;
public:
    //빠른 함수
    uint8_t version() const {return (version_hlen >> 4) & 0x0F; }
    uint8_t headerLength() const { return (version_hlen & 0x0F) * 4; }
    uint8_t protocolType() const { return protocol; }
    Ip const& sip() const { return src_ip; }
    Ip const& dip() const { return dst_ip; }

    //GETTER
    uint8_t getTos() const { return tos; }
    uint16_t getTolalLength() const { return total_length; }
    uint16_t getIdentifier() const { return identifier; }
    uint16_t getFlag() const { return flag; }
    uint8_t getTTL() const { return ttl; }
    uint8_t getProtocol() const { return protocol; }

    //SETTER
    void setVersionHlen(uint8_t version, uint8_t len);
    void setTos(uint8_t tos_);
    void setTotalLength(uint16_t total_length_);
    void setIdentifier(uint16_t identifier_);
    void setFlag(uint16_t flag_);
    void setTTL(uint8_t ttl_);
    void setProtocol(uint8_t protocol_);
    void setSrcDstIp(Ip src_ip_, Ip dst_ip_);





};
#pragma pack(pop)

enum IpProtocolType : uint8_t
{
    icmp = 1,
    tcp = 6,
    udp = 17
};
