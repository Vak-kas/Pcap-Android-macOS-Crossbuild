#include "packetmanager.h"

PacketDTO PacketManager::parsePacket(const QByteArray& packet)
{
    PacketDTO pkt;

    pkt.length = packet.size();

    if (packet.size() < sizeof(EthernetHeader))
    {
        pkt.proto = "UNKNOWN";
        pkt.src = "-";
        pkt.dst = "-";
        return pkt;
    }

    const EthernetHeader* eth = reinterpret_cast<const EthernetHeader*>(packet.data());

    uint16_t type = eth->type();

    // MAC 주소 (fallback용)
    pkt.src = QString::fromStdString(eth->smac().toString());
    pkt.dst = QString::fromStdString(eth->dmac().toString());

    if (type == EtherType::ipv4)
    {
        if (packet.size() < sizeof(EthernetHeader) + sizeof(IpHeader))
        {
            pkt.proto = "IPv4(short)";
            return pkt;
        }

        const IpHeader* ip =
            reinterpret_cast<const IpHeader*>(packet.data() + sizeof(EthernetHeader));

        // IP 주소
        pkt.src = QString::fromStdString(ip->sip().toString());
        pkt.dst = QString::fromStdString(ip->dip().toString());

        // 프로토콜
        switch (ip->protocolType())
        {
        case IpProtocolType::tcp:
            pkt.proto = "TCP";
            break;
        case IpProtocolType::udp:
            pkt.proto = "UDP";
            break;
        case IpProtocolType::icmp:
            pkt.proto = "ICMP";
            break;
        default:
            pkt.proto = "IPv4";
            break;
        }
    }
    else if (type == EtherType::arp)
    {
        pkt.proto = "ARP";
    }
    else if (type == EtherType::ipv6)
    {
        pkt.proto = "IPv6";
    }
    else
    {
        pkt.proto = QString("ETH(0x%1)").arg(type, 4, 16, QChar('0'));
    }

    return pkt;


}
