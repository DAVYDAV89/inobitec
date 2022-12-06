#ifndef GENERATOR_H
#define GENERATOR_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

class Generator : public QTcpServer
{
    Q_OBJECT
    QTimer *m_timerSend;
    QTcpSocket *m_socket;

    int m_amplitude{20};
public:
    Generator();
    ~Generator();

    bool startServer(quint16 _port);

protected:
    void incomingConnection(qintptr _handle);
};

#endif // GENERATOR_H
