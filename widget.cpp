#include "widget.h"
#include "ui_widget.h"
#include "pcapmanager.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    loadNICs();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::loadNICs()
{
    QList<NICInfo> nics = PcapManager::getNICList();

    ui->nicComboBox->clear();

    for (const NICInfo &nic : nics)
    {
        ui->nicComboBox->addItem(nic.name);
    }
}
