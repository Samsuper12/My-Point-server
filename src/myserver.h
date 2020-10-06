#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

#include "database.h"


struct testS{
    int a;
    int b;
};

class MyServer : public QObject
{
    Q_OBJECT
public:
    explicit MyServer(int nPort,QObject *parent = nullptr);

    void sendToClient(QTcpSocket* pSocket, const QString& str);
    void sendBaseToClient(QTcpSocket *pSocket);


private:
    DataBase m_dbClass;
    QTcpServer* m_ptcpServer;
    quint16  m_nNextBlockSize;

signals:

public slots:

    virtual void slotNewConnection();
            void slotReadClient   ();


private:

};

#endif // MYSERVER_H
