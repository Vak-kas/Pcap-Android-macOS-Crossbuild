#ifndef PACKETMANAGER_H
#define PACKETMANAGER_H

#include "ip.h"
#include "mac.h"
#include "ethernet.h"
#include "ipheader.h"
#include <pcap.h>
#include <vector>
#include "dto.h"
#include <string>


class PacketManager
{
public:
    PacketDTO parsePacket(const QByteArray& packet);
};


#endif // PACKETMANAGER_H
