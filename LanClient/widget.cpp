#include "widget.h"
#include "ui_widget.h"

#include <QTcpSocket>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    mSocket = new QTcpSocket(this);

    quint16 _port = 4444;
    mSocket->connectToHost("localhost", _port);

    ui -> color -> addItem(QString::fromLocal8Bit("Синий"));
    ui -> color -> addItem(QString::fromLocal8Bit("Красный"));
    ui -> color -> addItem(QString::fromLocal8Bit("Зеленый"));

    ui -> customPlot -> setInteraction(QCP::iRangeZoom, true);
    ui -> customPlot -> setInteraction(QCP::iRangeDrag, true);

    ui -> customPlot->axisRect()->setRangeDrag(Qt::Horizontal);
    ui -> customPlot->axisRect()->setRangeZoom(Qt::Horizontal);

    ui -> customPlot -> xAxis -> setRange(0, 100);
    ui -> customPlot -> yAxis -> setRange(0, 20);

    ui -> customPlot -> addGraph();

    connect(ui -> customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),
             this, SLOT(slotRangeChanged(QCPRange)));

    connect(mSocket, &QTcpSocket::connected, [&]() {
        qDebug() << "Client encrypted";
    });

    connect(mSocket, &QTcpSocket::readyRead, [&]() {
        QTextStream T(mSocket);
        auto text = T.readAll();

        x.push_back(_x);
        y.push_back(text.toInt());

        _x = _x + (ui -> spinBox -> text().toDouble());

        if ( _x > ui -> customPlot -> xAxis -> range().upper ) {
            ui -> customPlot -> xAxis -> setRange(0, _x);
            ui -> xUpDown -> setValue(_x);
        }

        if ( text.toInt() > ui -> customPlot -> yAxis -> range().upper ) {
            ui -> customPlot -> yAxis -> setRange(0, text.toInt());
        }

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
    if (ui -> bind -> text() == QString::fromLocal8Bit("Запустить")) {
        mSocket -> write("0x01");
        ui -> bind -> setText(QString::fromLocal8Bit("Остановить"));
    }
    else {
        mSocket -> write("0x00");
        ui -> bind -> setText(QString::fromLocal8Bit("Запустить"));
    }
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

void Widget::on_xUpDown_valueChanged(int _arg1)
{
    ui -> customPlot -> xAxis -> setRange(0, _arg1);
}

void Widget::on_width_valueChanged(int _arg1)
{
    QPen _pen = ui -> customPlot -> graph(0) -> pen();
    ui -> customPlot -> graph(0) -> setPen(_pen);

    _pen.setWidth(_arg1);
    ui -> customPlot -> graph(0) -> setPen(_pen);
}

void Widget::slotRangeChanged(QCPRange _range)
{
    qDebug() << ui -> customPlot -> xAxis -> range().upper;
    if (ui -> customPlot -> xAxis -> range().lower < 0)
        ui -> customPlot -> xAxis -> setRange(0, ui -> customPlot -> xAxis -> range().upper);

    ui -> xUpDown -> setValue(ui -> customPlot -> xAxis -> range().upper);
}
