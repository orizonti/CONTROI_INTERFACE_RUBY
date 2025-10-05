#ifndef CONNECTION_INTERFACE_H
#define CONNECTION_INTERFACE_H
#include <QObject>
#include <QHostAddress>
#include <qhostaddress.h>
class ConnectionInterface : public QObject
{
  //Q_OBJECT
  public:
  ConnectionInterface(QObject* parent = 0) : QObject(parent) {}; 
  virtual bool isMessageAvailable() = 0;
  virtual bool isConnected() = 0;
  virtual void ConnectTo(QString IPDevice, int Port) = 0;
  virtual void  ListenTo(QString IPDevice, int Port) = 0;
  virtual void  ListenTo(QHostAddress::SpecialAddress IPDevice, int Port) = 0;
  virtual void TryToConnectConstantly(QString address, int Port) = 0;

  public slots:
  virtual void SlotSendCommand(const QByteArray& ArrayCommand) = 0;
  virtual void SlotSendCommand(const char* DataCommand, int size) {};

  virtual void SlotCheckConnection() = 0;
  virtual void SlotCloseConnection() = 0;

};



#endif //CONNECTION_INTERFACE_H
