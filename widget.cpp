#include "widget.h"
#include "ui_widget.h"
#include "pcapmanager.h"
#include "QProcess"
#include <QStandardPaths>
#include <pcap.h>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    daemonManager = new DaemonManager(this);


    connect(daemonManager, &DaemonManager::nicDiscovered, this, &Widget::onNicDiscovered);

    connect(ui->nicComboBox, &NicComboBox::popupOpened,
            this, [this]()
            {
                ui->nicComboBox->clear();

            #ifdef Q_OS_ANDROID
                daemonManager->requestNICList();
            #else
                std::vector<std::string> nics = getNICList();
                for (const auto& nicName : nics)
                {
                    this->onNicDiscovered(QString::fromStdString(nicName));
                }
            #endif
            });

#ifdef Q_OS_ANDROID
    daemonManager->startDaemon();
#endif

}

Widget::~Widget()
{
    delete ui;
}


void Widget::onNicDiscovered(const QString &nic)
{
    ui->nicComboBox->addItem(nic);
}
