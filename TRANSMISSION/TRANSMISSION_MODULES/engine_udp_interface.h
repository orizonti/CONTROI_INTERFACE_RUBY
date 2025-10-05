#ifndef ENGINE_UDP_INTERFACE_H
#define ENGINE_UDP_INTERFACE_H

#include <QDebug>

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QTextStream>
#include <QAbstractSocket>
#include <QProcess>
#include <QFile>

#include <connection_interface.h>
#include <QTimer>

#include "engine_ring_buffer_generic.h"
#include "message_command_structures.h"

class UDPEngineInterface : public ConnectionInterface
{
    Q_OBJECT
public:
    explicit UDPEngineInterface(QString IPDevice, int Port,QString IPListen, int PortLocal, QObject *parent = nullptr);
             UDPEngineInterface(QString IPDevice, int Port,QHostAddress::SpecialAddress IPListen, int PortLocal, QObject *parent = nullptr);
    ~UDPEngineInterface();

    bool isMessageAvailable() override;
    bool isConnected() override;
    void ConnectTo(QString IPDevice, int Port) override;
    void  ListenTo(QString IPHost, int Port) override;
    void  ListenTo(QHostAddress::SpecialAddress IPHost, int Port) override;
    void TryToConnectConstantly(QString Address, int Port) override;
    int DataCounter = 0;

public slots:
  void SlotSendCommand(const QByteArray& ArrayCommand) override;
  void SlotSendCommand(const char* DataCommand, int size) override;
  void SlotCheckConnection() override;
  void SlotCloseConnection() override;

private:
   void SendCloseConnectionCommand();

   QUdpSocket* Socket = nullptr;
   QTimer timerConnectAttempt;

private slots:
   void SlotReadData();
   void SlotCheckRequest();

public: 
    QString IPRemote;
        int PortRemote = 7575;
        int PortLocal  = 7575;
       bool Connected  = false;

RingBufferGeneric<MESSAGE_HEADER_GENERIC,sizeof(MessageMeasure1), 4,IteratorMode::Continous>* RingBuffer = nullptr;

signals:
void SignalDeviceConnected();
void SignalMessageAvailable();

};



#endif // UDPEngineInterface_H
