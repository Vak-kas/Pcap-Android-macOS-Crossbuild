#include "widget.h"
#include "ui_widget.h"


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
    ui->tableView->verticalHeader()->setVisible(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->pushButton_3, &QPushButton::clicked, this, [this]()
            {

                PacketDTO pkt;
                pkt.time = "12:00:00";
                pkt.src = "192.168.0.1";
                pkt.dst = "192.168.0.2";
                pkt.proto = "TCP";
                pkt.length = 60;

                addPacketRow(pkt);
            });
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


void Widget::addPacketRow(const PacketDTO &pkt)
{
    QList<QStandardItem*> row;

    row << new QStandardItem(pkt.time);
    row << new QStandardItem(pkt.src);
    row << new QStandardItem(pkt.dst);
    row << new QStandardItem(pkt.proto);
    row << new QStandardItem(QString::number(pkt.length));

    model->appendRow(row);
    ui->tableView->scrollToBottom();
}
