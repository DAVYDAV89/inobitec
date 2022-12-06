#include "widget.h"
#include "ui_widget.h"

#include <QTcpSocket>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    mSocket = new QTcpSocket(this);

    ui -> color -> addItem(QString::fromLocal8Bit("Синий"));
    ui -> color -> addItem(QString::fromLocal8Bit("Красный"));
    ui -> color -> addItem(QString::fromLocal8Bit("Зеленый"));

    ui -> customPlot -> setInteraction(QCP::iRangeZoom, true);

    ui -> customPlot -> xAxis -> setRange(0, 100);
    ui -> customPlot -> yAxis -> setRange(0, 20);

    ui -> customPlot -> addGraph();

    connect(mSocket, &QTcpSocket::connected, [&]() {
        qDebug() << "Client encrypted";
        mSocket -> write(QString::number(20).toLatin1());
    });

    connect(mSocket, &QTcpSocket::readyRead, [&]() {
        QTextStream T(mSocket);
        auto text = T.readAll();

        x.push_back(_x);
        y.push_back(text.toInt());

        _x = _x + (ui -> spinBox -> text().toDouble());

        ui -> customPlot -> graph(0) -> addData(x,y);
        ui -> customPlot -> replot();
    });
}

Widget::~Widget()
{
    delete mSocket;
    delete ui;
}

void Widget::on_bind_clicked()
{
//    QString _str;
//    QFile _setPort("../LanClient/setPort.conf");
//    if ( (_setPort.exists()) && (_setPort.open(QIODevice::ReadOnly) ) ){

//        while(!_setPort.atEnd())
//        {
//            _str += _setPort.readLine();
//        }
//        _setPort.close();
//    }

//    quint16 _port;
//    QStringList _arguments = _str.split("port:", QString::SkipEmptyParts);
//    for (const auto &el : _arguments) {
//        _port = el.toUInt();
//    }

    quint16 _port = 4444;
    mSocket->connectToHost("localhost", _port);
}

void Widget::on_color_activated(int _index)
{
    switch (_index) {
    case 0:
        ui -> customPlot -> graph(0) -> setPen(QPen(QColor(Qt::blue)));
        break;
    case 1:
        ui -> customPlot -> graph(0) -> setPen(QPen(QColor(Qt::red)));
        break;
    case 2:
        ui -> customPlot -> graph(0) -> setPen(QPen(QColor(Qt::green)));
        break;
    }

    on_width_valueChanged(ui -> width -> text().toInt());
}

void Widget::on_amplitude_valueChanged(int _arg1)
{
    mSocket -> write(QString::number(_arg1).toLatin1());
}

void Widget::on_xUpDown_valueChanged(int _arg1)
{
    ui -> customPlot -> xAxis -> setRange(0, _arg1);
}

void Widget::on_yUpDown_valueChanged(int _arg1)
{
    ui -> customPlot -> yAxis -> setRange(0, _arg1);
}

void Widget::on_width_valueChanged(int _arg1)
{
    QPen _pen = ui -> customPlot -> graph(0) -> pen();
    ui -> customPlot -> graph(0) -> setPen(_pen);

    _pen.setWidth(_arg1);
    ui -> customPlot -> graph(0) -> setPen(_pen);
}
