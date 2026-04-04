#include "widget.h"
#include "ui_widget.h"
#include "pcapmanager.h"
#include "QProcess"
#include <QStandardPaths>
#include "daemonmanager.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    daemonManager = new DaemonManager(this);

    connect(daemonManager, &DaemonManager::nicDiscovered,
            this, [this](QString nic)
            {
                if (ui->nicComboBox->findText(nic) == -1)
                    ui->nicComboBox->addItem(nic);
            });

    daemonManager->startDaemon();
}

Widget::~Widget()
{
    delete ui;
}
