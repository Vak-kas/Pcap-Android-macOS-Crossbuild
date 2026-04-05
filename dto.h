#ifndef PACKETDTO_H
#define PACKETDTO_H

#include <QString>

struct PacketDTO
{
    QString time;
    QString src;
    QString dst;
    QString proto;
    int length;
};

#endif
