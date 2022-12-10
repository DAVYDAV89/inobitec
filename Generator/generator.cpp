#include "generator.h"
#include <QRandomGenerator>

Generator::Generator()
{
    m_timerSend = new QTimer;
    connect(m_timerSend, &QTimer::timeout, [&]() {
        m_socket -> write(QString::number(QRandomGenerator::global() -> bounded(0, m_amplitude)).toLatin1());
    });
}

Generator::~Generator()
{
    delete m_timerSend;
}

bool Generator::startServer(quint16 _port)
{
    return listen(QHostAddress::Any, _port);
}

void Generator::incomingConnection(qintptr _handle)
{
    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::readyRead, [&]() {
        QTextStream T(m_socket);
        auto text = T.readAll();

        if (text == "0x01")
            m_timerSend -> start(200);
        else if (text == "0x00") {
            if (m_timerSend -> isActive())
                m_timerSend -> stop();
        }




//        m_amplitude = text.toInt();
    });

    if (m_socket->setSocketDescriptor(_handle)) {
        qDebug() << "Client connected with handle: " << _handle;
    }
}
