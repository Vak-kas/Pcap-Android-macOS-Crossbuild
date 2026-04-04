#include "daemonmanager.h"
#include <QDebug>

DaemonManager::DaemonManager(QObject *parent)
    : QObject(parent), process(nullptr)
{
}

void DaemonManager::startDaemon()
{
    if (process)
    {
        if (process->state() == QProcess::Running)
            return;

        delete process;
        process = nullptr;
    }

    process = new QProcess(this);

    connect(process, &QProcess::readyReadStandardOutput,
            this, &DaemonManager::onDaemonOutput);

    connect(process, &QProcess::readyReadStandardError,
            this, &DaemonManager::onDaemonError);

    QString daemonName = "mjDaemon";
    QString daemonPath = "/data/local/tmp/" + daemonName;

    QStringList args;
    args << "-c" << daemonPath;

    process->start("su", args);
    if (!process->waitForStarted())
    {
        qDebug() << "Daemon start failed";
        return;
    }
}

void DaemonManager::requestNICList()
{
    if (process && process->state() == QProcess::Running)
    {
        process->write("GET_NIC\n");
    }
}

void DaemonManager::onDaemonOutput()
{
    QByteArray data = process->readAllStandardOutput();
    QString text = QString::fromUtf8(data);

    QStringList lines = text.split("\n");

    for (QString line : lines)
    {
        if (line.startsWith("NIC:"))
        {
            QString nic = line.mid(4);
            emit nicDiscovered(nic);
        }
    }
}


void DaemonManager::onDaemonError()
{
    QByteArray data = process->readAllStandardError();
    QString text = QString::fromUtf8(data);

    qDebug() << "[DAEMON ERROR]" << text;
}
