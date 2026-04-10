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
    connect(ui->nicComboBox, &NicComboBox::popupOpened, this, &Widget::onNicPopupOpened);
    connect(ui->nicComboBox, &QComboBox::currentTextChanged, this, &Widget::onNicSelected);


    // -----------Capture Control Button-----------
    connect(ui->startButton, &QPushButton::clicked,
            this, &Widget::onStartClicked);

    connect(daemonManager, &DaemonManager::packetReceived,
            this, &Widget::onPacketReceived);

    connect(ui->stopButton, &QPushButton::clicked,
            this, &Widget::onStopClicked);

    connect(ui->resetButton, &QPushButton::clicked,
            this, &Widget::onResetClicked);

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

void Widget::onNicPopupOpened()
{
#ifdef Q_OS_ANDROID
    daemonManager->requestNICList();
#else
    loadNICList();
#endif

}

void Widget::loadNICList()
{
    ui->nicComboBox->clear();

    std::vector<std::string> nics = getNICList();
    for (const auto& nicName : nics)
    {
        onNicDiscovered(QString::fromStdString(nicName));
    }
}

void Widget::onNicSelected(const QString& nic)
{
    selectedNic = nic;
    qDebug() << "Selected NIC: " << nic;
}

void Widget::onStartClicked()
{
    if (selectedNic.isEmpty())
    {
        qDebug() << "No NIC selected";
        return;
    }

    if (isCapturing)
    {
        qDebug() << "Already capturing";
        return;
    }

#ifdef Q_OS_ANDROID
    daemonManager->startCapture(selectedNic);
    isCapturing = true;
#else
    //macos 캡처
#endif

}

void Widget::onStopClicked()
{
#ifdef Q_OS_ANDROID
    daemonManager->stopCapture();
#else
    //masos
#endif

    isCapturing = false;
}

void Widget::onResetClicked()
{
    model->removeRows(0, model->rowCount());
    model->setHorizontalHeaderLabels({
        "Time", "Source", "Destination", "Protocol", "Length"
    });
}



void Widget::onPacketReceived(const QByteArray& data)
{
    qDebug() << "Packet received:" << data.size();

    PacketDTO pkt;
    pkt.time = QTime::currentTime().toString("HH:mm:ss");
    pkt.src = "-";
    pkt.dst = "-";
    pkt.proto = "RAW";
    pkt.length = data.size();

    addPacketRow(pkt);
}
