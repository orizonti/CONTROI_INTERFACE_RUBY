#ifndef TCP_ENGINE_INTERFACE_H
#define TCP_ENGINE_INTERFACE_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QImage>
#include <QElapsedTimer>

#include <QMutex>
#include <QThread>
#include <QByteArray>
#include <QBuffer>
#include <QTimer>
#include "engine_ring_buffer_generic.h"
#include "connection_interface.h"


class TCPConnectionEngine : public ConnectionInterface
{
    Q_OBJECT
public:
  explicit TCPConnectionEngine(QObject *parent = nullptr);
  ~TCPConnectionEngine();
  using BufferType = RingBufferGeneric<MESSAGE_HEADER_GENERIC,50, 1000,IteratorMode::Continous>; 

public:
  bool isMessageAvailable();
  bool isConnected();
  void ConnectTo(QString IPDevice, int Port);
  void  ListenTo(QString IPDevice, int Port);
  void  ListenTo(QHostAddress::SpecialAddress IPDevice, int Port);
  void TryToConnectConstantly(QString Address, int Port);

public slots:
  void SlotSendCommand(const QByteArray& ArrayCommand);
  void SlotCheckConnection();
  void SlotCloseConnection();

private slots:
  void SlotReadData();
  void SlotConnectedToHost();
  void SlotConnectionAttempt();
  void SlotAcceptConnection();

private:
  void SendCloseConnectionCommand();

  QTcpSocket* Socket = 0;
  QTcpServer* Server = 0;
  QTimer      TimerAutoconnection;
//=================================================================
public:
    QString IPRemote = "127.0.0.1";
        int PortRemote = 2325;
    QString IPLocal = "127.0.0.1";
        int PortLocal = 2325;
    BufferType* RingBuffer = 0;

  signals:
  void SignalDeviceConnected();
  void SignalMessageAvailable();
};


#endif // TCP_ENGINE_INTERFACE_H
