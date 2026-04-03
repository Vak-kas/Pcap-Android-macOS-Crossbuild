#ifndef PCAPMANAGER_H
#define PCAPMANAGER_H

#include <QString>
#include <QList>

struct NICInfo
{
    QString name;        // 실제 NIC (pcap에서 사용)
    QString description; // UI 표시용
};

class PcapManager
{
public:
    static QList<NICInfo> getNICList();
};



#endif // PCAPMANAGER_H
