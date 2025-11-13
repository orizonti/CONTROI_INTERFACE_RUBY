#include "engine_udp_interface.h"
#include "connection_interface.h"
#include <qhostaddress.h>
#include "debug_output_filter.h"


UDPConnectionEngine::UDPConnectionEngine(QString IPDevice, int Port, QString IPListen, int PortLocal, QObject *parent)
    : ConnectionInterface{parent}
{

    qDebug() << "[ UDP CONNECTION ]" << IPDevice << Port << IPListen << PortLocal;
    connectTo(IPDevice, Port); 
    listenTo(IPListen, PortLocal);

    RingBuffer = new std::remove_reference<decltype((*RingBuffer))>::type;
    QObject::connect(Socket,SIGNAL(readyRead()),this,SLOT(slotReadData()));
}

UDPConnectionEngine::UDPConnectionEngine(QString IPDevice, int Port,QHostAddress::SpecialAddress IPListen, int PortLocal, QObject *parent)
 : ConnectionInterface(parent)
{
    connectTo(IPDevice, Port); 
    listenTo(IPListen, PortLocal);
    qDebug() << "[ UDP CONNECTION ]" << IPDevice << Port << IPListen << PortLocal;

    RingBuffer = new std::remove_reference<decltype((*RingBuffer))>::type;
    QObject::connect(Socket,SIGNAL(readyRead()),this,SLOT(slotReadData()));
}

UDPConnectionEngine::UDPConnectionEngine(QObject* parent) : ConnectionInterface(parent)
{
    qDebug() << "[ UDP CONNECTION NEED SET SOCKET]" ;
    RingBuffer = new std::remove_reference<decltype((*RingBuffer))>::type;
    QObject::connect(Socket,SIGNAL(readyRead()),this,SLOT(slotReadData()));
}

void UDPConnectionEngine::connectTo(QString IPDevice, int Port) 
{
    if(isConnected()) slotCloseConnection();

    if(Socket == nullptr) { Socket = new QUdpSocket(this); 
                            Socket->open(QIODevice::ReadWrite); }

    PortRemote = Port; IPRemote = IPDevice;
   if(!Socket->isOpen()) Socket->open(QIODevice::ReadWrite);
};

void  UDPConnectionEngine::listenTo(QString IPHost, int Port)
{
    if(Socket == nullptr) { Socket = new QUdpSocket(this); 
                            Socket->open(QIODevice::ReadWrite); }
       Socket->bind(QHostAddress(IPHost), Port); 
       Socket->flush();
   if(!Socket->isOpen()) Socket->open(QIODevice::ReadWrite);
}
void  UDPConnectionEngine::listenTo(QHostAddress::SpecialAddress IPHost, int Port)
{
    if(Socket == nullptr) { Socket = new QUdpSocket(this); 
                            Socket->open(QIODevice::ReadWrite); }
       //Socket->bind(IPHost, Port); 
       Socket->bind(QHostAddress::Any, Port); 
       Socket->flush();
    if(!Socket->isOpen()) Socket->open(QIODevice::ReadWrite);
}

UDPConnectionEngine::~UDPConnectionEngine()
{
 qDebug() << "DELETE UDP INTERFACE";
 delete Socket;
}

void UDPConnectionEngine::slotReadData()
{
   if(Socket->bytesAvailable() < RingBuffer->MIN_MESSAGE_SIZE) return;

      auto Datagram = Socket->receiveDatagram();
      DataCounter += Datagram.data().size();
      //qDebug() << "READ DATA : " << DataCounter;

      RingBuffer->AppendData((uint8_t*)Datagram.data().data(), Datagram.data().size());

   if(RingBuffer->isMessageAvailable()) emit signalMessageAvailable();

   if(Socket->bytesAvailable() > 24) slotReadData();
}


void UDPConnectionEngine::slotSendMessage(const QByteArray& Command, uint8_t Param)
{
 //qDebug() << OutputFilter::Filter(100) << "SEND COMMAND: " << QString(Command.toHex());
 Socket->writeDatagram(Command,QHostAddress(IPRemote),PortRemote); 
}

void UDPConnectionEngine::slotSendMessage(const char* Command, int size, uint8_t Param)
{
    Socket->writeDatagram(Command,size,QHostAddress(IPRemote), PortRemote);
}



void UDPConnectionEngine::slotCloseConnection()
{
    Connected = false; Socket->close();
}


bool UDPConnectionEngine::isConnected() { return Connected;}

void UDPConnectionEngine::tryConnectConstantly(QString Address, int Port)
{
   Connected = false; 

   RingBuffer->Reset(); connectTo(Address, Port); slotCheckConnection();

   QTimer::singleShot(2000, this, SLOT(slotCheckConnection()));
}

void UDPConnectionEngine::slotCheckConnection()
{
  if(RingBuffer->isMessageAvailable()) 
  {
  qDebug() << "CONNECTED TO : " << IPRemote << ":" << PortRemote; 
  Connected = true; 
  }

  qDebug() << "TRY TO CONNECT TO : " << IPRemote << ":" << PortRemote; 
  QTimer::singleShot(2000, this, SLOT(slotCheckConnection()));
}


bool UDPConnectionEngine::isMessageAvailable() { return RingBuffer->isMessageAvailable(); }
