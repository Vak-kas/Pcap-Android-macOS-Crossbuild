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


    //---------------------Daemon---------------------
    connect(daemonManager, &DaemonManager::nicDiscovered, this, &Widget::onNicDiscovered);
    #ifdef Q_OS_ANDROID
        daemonManager->startDaemon();
    #endif


    //---------------------NIC---------------------
    connect(ui->nicComboBox, &NicComboBox::popupOpened,
            this, [this]()
            {
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

    //---------------------Table---------------------
    model = new QStandardItemModel(this);
    model->setColumnCount(5);
    model->setHorizontalHeaderLabels({
        "Time", "Source", "Destination", "Protocol", "Length"
    });
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);





}

Widget::~Widget()
{
    delete ui;
}


void Widget::onNicDiscovered(const QString &nic)
{
    // 중복 추가 방지 및 로그 확인
    if (ui->nicComboBox->findText(nic) == -1)
    {
        ui->nicComboBox->addItem(nic);
        qDebug() << "UI added:" << nic << "Total count:" << ui->nicComboBox->count();
    }
}
