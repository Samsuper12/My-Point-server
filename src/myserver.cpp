#include "myserver.h"

MyServer::MyServer(int nPort, QObject *parent) : QObject(parent), m_nNextBlockSize(0)
{
    m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::Any, nPort)) {
        qDebug() << "Server Error" << "Unable to start the server: " << m_ptcpServer->errorString();

        m_ptcpServer->close();
        return;
    }

    connect(m_ptcpServer, SIGNAL(newConnection()),
            this,         SLOT(slotNewConnection())
           );

    m_dbClass.connectToDataBase();
    m_dbClass.insertIntoMarkerTable(2.5,2.0, "Login", "Descripton", 0);
    m_dbClass.insertIntoMarkerTable(3.5,3.0, "Login", "Descripton", 2);
    m_dbClass.insertIntoMarkerTable(4.5,4.0, "Login", "Descripton", 1);
    m_dbClass.insertIntoMarkerTable(5.5,5.0, "Login", "Descripton", 2);
    m_dbClass.insertIntoMarkerTable(6.5,6.0, "Login", "Descripton", 1);
    m_dbClass.insertIntoUserTable("Mishael", "12345");
    m_dbClass.testfunk();
    qDebug() << "!!!---------------------!!!";
    bool ok =m_dbClass.findUserInTable("Mishael", "123u45");

    qDebug() << ok << "RESULT";


}

void MyServer::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater()));

    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient()));

    sendToClient(pClientSocket, "Server Response: Connected!");
}

void MyServer::slotReadClient()
{
    m_nNextBlockSize = 0;
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }

        QTime   time;
        QString str;

        in >> time >> str;

        QString strMessage = time.toString() + " " + "Client has sended - " + str;
        qDebug() << strMessage;


       if(str == "Synchronize"){

           m_nNextBlockSize = 0;

           sendBaseToClient(pClientSocket);
       }
       else if(str == "CreateUserData"){

           QJsonDocument qjd; // and another operations
           QJsonObject qjo;
           QByteArray data;

           in >> data;

           qjd = qjd.fromJson(data);
           qjo = qjd.object();

           m_dbClass.insertIntoUserTable(qjo.take("login").toString(), qjo.take("password").toString()); // ДОБАВИТЬ ПРОВЕРКУ // МОЖЕТ БЫТЬ ПОТЕРЯ ДАННЫХ ИЗ-ЗА TAKE
           sendToClient(pClientSocket, "UserHasAdded");

           m_nNextBlockSize = 0;

       }
       else if(str == "ProveUserData"){
           QJsonDocument qjd; // and another operations
           QJsonObject qjo;
           QByteArray data;
           QString logbuf, passbuf;

           in >> data;

           qjd = qjd.fromJson(data);
           qjo = qjd.object();

           logbuf =  qjo.take("login").toString();
           passbuf = qjo.take("password").toString();


          if(m_dbClass.findUserInTable(logbuf, passbuf)){
              sendToClient(pClientSocket, "UserHasAlready");

           }
           else {
               sendToClient(pClientSocket, "UserIsntFind");

           }

            m_nNextBlockSize = 0;
       }

       else if (str == "ClentSendMarker") {

           QJsonDocument qjd;
           QJsonObject qjo;
           QByteArray data;
           QString logbuf, passbuf;

           in >> data;

           qjd = qjd.fromJson(data);
           qjo = qjd.object();

           m_dbClass.insertIntoMarkerTable(qjo.take("latitude").toDouble(),qjo.take("longitude").toDouble(),
                                           qjo.take("userLogin").toString(), qjo.take("description").toString(),
                                           qjo.take("danger").toInt());
           qDebug() << "new marker from client";
           m_nNextBlockSize = 0;
       }

       else{

           m_nNextBlockSize = 0;
           sendToClient(pClientSocket,  "Server Response: Received \"" + str + "\"");

       }
    }

        m_nNextBlockSize = 0;

}

void MyServer::sendToClient(QTcpSocket *pSocket, const QString &str)
{

    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);

    out << quint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);

}

void MyServer::sendBaseToClient(QTcpSocket *pSocket)
{
    QByteArray  arrBlock;
    QString str("SynchronizeResponse");
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);

    QJsonDocument jsondoc = m_dbClass.getAllBadeJson();

    out << quint16(0) << QTime::currentTime() << str << jsondoc.toJson();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    QJsonArray ar = jsondoc.array();
    qDebug() << ar.count();

    pSocket->write(arrBlock);

}

