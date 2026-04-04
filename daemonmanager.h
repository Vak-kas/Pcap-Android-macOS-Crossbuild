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

signals:
    void nicDiscovered(QString nic);

private slots:
    void onDaemonOutput();
    void onDaemonError();

private:
    QProcess *process;
};

#endif // DAEMONMANAGER_H
