#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_bind_clicked();

    void on_color_activated(int _index);
    void on_amplitude_valueChanged(int _arg1);

    void on_xUpDown_valueChanged(int _arg1);
    void on_yUpDown_valueChanged(int _arg1);

    void on_width_valueChanged(int _arg1);

private:
    Ui::Widget *ui;
    QTcpSocket *mSocket;

    int _x{0};

    QVector<double> x,y;

};
#endif // WIDGET_H
