#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>

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
    void startRootDaemon();
    void onDaemonOutput();
    void onDaemonError();

private:
    Ui::Widget *ui;

    QProcess *daemonProcess;

};
#endif // WIDGET_H
