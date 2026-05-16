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
    void disableMonitorMode(const QString& nic);
    void enableMonitorMode(const QString& nic);

signals:
    void nicDiscovered(QString nic);
    void packetReceived(QByteArray data);
    void daemonLog(const QString& msg);
    void daemonError(const QString& msg);


private slots:
    void onDaemonOutput();
    void onDaemonError();


private:
    QProcess *process;
    QByteArray buffer;
};

#endif // DAEMONMANAGER_H
