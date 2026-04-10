#ifndef DAEMONMANAGER_H
#define DAEMONMANAGER_H
#pragma once
#include <QObject>
#include <QProcess>

class DaemonManager : public QObject
{
    Q_OBJECT

public:
    explicit DaemonManager(QObject *parent = nullptr);
    ~DaemonManager();

    void startDaemon();
    void requestNICList();
    void startCapture(const QString& nic);
    void stopCapture();

signals:
    void nicDiscovered(QString nic);
    void packetReceived(QByteArray data);


private slots:
    void onDaemonOutput();
    void onDaemonError();

private:
    QProcess *process;
    QByteArray buffer;
};

#endif // DAEMONMANAGER_H
