#pragma once
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>
#include <QComboBox>
#include <QStandardItemModel>
#include "pcapmanager.h"
#include <QStandardPaths>
#include <pcap.h>
#include "daemonmanager.h"
#include "dto.h"
#include <thread>
#include <QTime>




QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void onNicDiscovered(const QString &nic);
    void addPacketRow(const PacketDTO &pkt);
    void onNicPopupOpened();
    void loadNICList();
    void onNicSelected(const QString& nic);

    void onStartClicked();
    void onStopClicked();
    void onResetClicked();

    void onPacketReceived(const QByteArray& data);

private:
    Ui::Widget *ui;
    DaemonManager *daemonManager;
    QStandardItemModel *model;
    QString selectedNic;
    bool isCapturing = false;
};


class NicComboBox : public QComboBox
{
    Q_OBJECT

public:
    using QComboBox::QComboBox;

signals:
    void popupOpened();

protected:
    void showPopup() override
    {
        emit popupOpened();
        QComboBox::showPopup();
    }
};




#endif // WIDGET_H

