#include "daemonmanager.h"
#include <QDebug>
#include "common.h"

DaemonManager::DaemonManager(QObject *parent)
    : QObject(parent), process(nullptr)
{
}

void DaemonManager::startDaemon()
{
#ifdef Q_OS_ANDROID
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



    process->start("su", QStringList() << "-c" << "/data/local/tmp/mjDaemon");

    if (!process->waitForStarted()) {
        qDebug() << "su execution failed";
        return;
    }
#else
    qDebug() << "Native OS (macOS/Windows): Daemon start skipped.";
#endif
}

void DaemonManager::requestNICList()
{
    if (process && process->state() == QProcess::Running)
    {
        process->write("GET_NIC\n");
    }
}

void DaemonManager::startCapture(const QString& nic)
{
    if (process && process->state() == QProcess::Running)
    {
        QString cmd = "START_CAPTURE " + nic + "\n";
        process->write(cmd.toUtf8());
    }
}

void DaemonManager::stopCapture()
{
    if (process && process->state() == QProcess::Running)
    {
        process->write("STOP_CAPTURE\n");
    }
}


void DaemonManager::onDaemonOutput()
{
    buffer.append(process->readAllStandardOutput());

    // qint64와 size_t 비교 시 안전하게 int로 캐스팅
    while (buffer.size() >= (int)sizeof(MsgHeader))
    {
        MsgHeader header;
        memcpy(&header, buffer.data(), sizeof(MsgHeader));

        // 데이터가 쪼개져서 왔을 때를 대비한 핵심 로직
        if (buffer.size() < (int)(sizeof(MsgHeader) + header.length))
        {
            break;
        }

        QByteArray payload = buffer.mid(sizeof(MsgHeader), header.length);
        buffer.remove(0, sizeof(MsgHeader) + header.length);

        if (header.type == MSG_NIC)
        {
            QString nic = QString::fromUtf8(payload);
            qDebug() << "[SUCCESS] Emitting NIC:" << nic;
            emit nicDiscovered(nic);
        }
        else if (header.type == MSG_PKT)
        {
            qDebug() << "[PKT] size:" << header.length;
            emit packetReceived(payload);
        }
        else if (header.type == MSG_LOG)
        {
            QString msg = QString::fromUtf8(payload);
            qDebug() << "[DAEMON LOG]" << msg;
            emit daemonLog(msg);
        }
        else if (header.type == MSG_ERR)
        {
            QString msg = QString::fromUtf8(payload);
            qDebug() << "[DAEMON ERROR MSG]" << msg;
            emit daemonError(msg);
        }
    }
}

void DaemonManager::onDaemonError()
{
    QByteArray data = process->readAllStandardError();
    QString text = QString::fromUtf8(data);

    qDebug() << "[DAEMON ERROR]" << text;
}

DaemonManager::~DaemonManager()
{
#ifdef Q_OS_ANDROID
    if (process && process->state() == QProcess::Running)
    {
        process->write("EXIT\n");
        process->waitForBytesWritten();
        process->waitForFinished(1000);
        process->kill(); // fallback
    }
#endif
}


void DaemonManager::enableMonitorMode(const QString& nic)
{
    if (process && process->state() == QProcess::Running)
    {
        QString cmd = "MONITOR_ON " + nic + "\n";
        qDebug() << "[SEND]" << cmd;
        process->write(cmd.toUtf8());
        process->waitForBytesWritten();
    }
}

void DaemonManager::disableMonitorMode(const QString& nic)
{
    if (process && process->state() == QProcess::Running)
    {
        QString cmd = "MONITOR_OFF " + nic + "\n";
        process->write(cmd.toUtf8());
    }
}
