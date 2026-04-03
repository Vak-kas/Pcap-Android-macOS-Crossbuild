#include "widget.h"
#include "ui_widget.h"
#include "pcapmanager.h"
#include "QProcess"
#include <QStandardPaths>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , daemonProcess(nullptr)
{
    ui->setupUi(this);
    startRootDaemon();
}

Widget::~Widget()
{
    if (daemonProcess && daemonProcess->state() == QProcess::Running)
    {
        daemonProcess->kill();              // 프로세스 강제 종료
        daemonProcess->waitForFinished();   // 완전히 종료될 때까지 대기
    }

    delete daemonProcess;   // 메모리 해제
    delete ui;
}


void Widget::startRootDaemon()
{
    if (daemonProcess && daemonProcess->state() == QProcess::Running)
    {
        return;
    }

    if (daemonProcess)
    {
        delete daemonProcess;
        daemonProcess = nullptr;
    }

    daemonProcess = new QProcess(this);
    QString daemonName = "mjDaemon";

    // connect(daemonProcess, &QProcess::readyReadStandardOutput,
    //         this, &Widget::onDaemonOutput);

    // connect(daemonProcess, &QProcess::readyReadStandardError,
    //         this, &Widget::onDaemonError);

    QString daemonPath = "/data/local/tmp/" + daemonName;

    QStringList args;
    args << "-c" << daemonPath;

    daemonProcess->start("su", args);
}
