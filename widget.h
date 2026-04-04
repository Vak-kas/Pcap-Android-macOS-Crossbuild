#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>
#include "daemonmanager.h"
#include <QComboBox>
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

private:
    Ui::Widget *ui;
    DaemonManager *daemonManager;

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

