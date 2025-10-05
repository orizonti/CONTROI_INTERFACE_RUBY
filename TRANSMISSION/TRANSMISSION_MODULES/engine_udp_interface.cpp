#include "engine_udp_interface.h"
#include "connection_interface.h"
#include <qhostaddress.h>



UDPEngineInterface::UDPEngineInterface(QString IPDevice, int Port, QString IPListen, int PortLocal, QObject *parent)
    : ConnectionInterface{parent}
{

    qDebug() << "[ UDP CONNECTION ]" << IPDevice << Port << IPListen << PortLocal;
    ConnectTo(IPDevice, Port); 
    ListenTo(IPListen, PortLocal);

    RingBuffer = new std::remove_reference<decltype((*RingBuffer))>::type;
    QObject::connect(Socket,SIGNAL(readyRead()),this,SLOT(SlotReadData()));

    //MessageGenericExt<MessagePositionState  ,MESSAGE_HEADER> StateScanator;
    //MessageGenericExt<MessageMeasureSeries<10000>,MESSAGE_HEADER_EXT> MessageMeasure;

}

UDPEngineInterface::UDPEngineInterface(QString IPDevice, int Port,QHostAddress::SpecialAddress IPListen, int PortLocal, QObject *parent)
 : ConnectionInterface(parent)
{
    ConnectTo(IPDevice, Port); 
    ListenTo(IPListen, PortLocal);
    qDebug() << "[ UDP CONNECTION ]" << IPDevice << Port << IPListen << PortLocal;

    RingBuffer = new std::remove_reference<decltype((*RingBuffer))>::type;
    QObject::connect(Socket,SIGNAL(readyRead()),this,SLOT(SlotReadData()));
}

void UDPEngineInterface::ConnectTo(QString IPDevice, int Port) 
{
    if(isConnected()) SlotCloseConnection();

    if(Socket == nullptr) { Socket = new QUdpSocket(this); 
                            Socket->open(QIODevice::ReadWrite); }

    PortRemote = Port; IPRemote = IPDevice;
};

void  UDPEngineInterface::ListenTo(QString IPHost, int Port)
{
    if(Socket == nullptr) { Socket = new QUdpSocket(this); 
                            Socket->open(QIODevice::ReadWrite); }
       Socket->bind(QHostAddress(IPHost), Port); 
       Socket->flush();
}
void  UDPEngineInterface::ListenTo(QHostAddress::SpecialAddress IPHost, int Port)
{
    if(Socket == nullptr) { Socket = new QUdpSocket(this); 
                            Socket->open(QIODevice::ReadWrite); }
       //Socket->bind(IPHost, Port); 
       Socket->bind(QHostAddress::Any, 1313); 
       Socket->flush();
}

UDPEngineInterface::~UDPEngineInterface()
{
 qDebug() << "DELETE UDP INTERFACE";
 delete Socket;
}

void UDPEngineInterface::SlotReadData()
{
   if(Socket->bytesAvailable() < RingBuffer->MIN_MESSAGE_SIZE) return;

      auto Datagram = Socket->receiveDatagram();
      DataCounter += Datagram.data().size();
      //qDebug() << "READ DATA : " << DataCounter;

      RingBuffer->AppendData((uint8_t*)Datagram.data().data(), Datagram.data().size());

   if(RingBuffer->isMessageAvailable()) emit SignalMessageAvailable();

   if(Socket->bytesAvailable() > 24) SlotReadData();
}


void UDPEngineInterface::SlotSendCommand(const QByteArray& ArrayCommand)
{
 Socket->writeDatagram(ArrayCommand,QHostAddress(IPRemote),PortRemote); 
 //Socket->waitForBytesWritten(5);
}

void UDPEngineInterface::SlotSendCommand(const char* DataCommand, int size)
{
    Socket->writeDatagram(DataCommand,size,QHostAddress(IPRemote), PortRemote);
}


void UDPEngineInterface::SlotCloseConnection()
{
    SendCloseConnectionCommand();
    Connected = false; Socket->close();
}


bool UDPEngineInterface::isConnected() { return Connected;}

void UDPEngineInterface::TryToConnectConstantly(QString Address, int Port)
{
   Connected = false; 

   RingBuffer->Reset(); ConnectTo(Address, Port); SlotCheckConnection();

   QTimer::singleShot(2000, this, SLOT(SlotCheckRequest()));
}

void UDPEngineInterface::SlotCheckRequest()
{
  if(RingBuffer->isMessageAvailable()) 
  {
  qDebug() << "CONNECTED TO : " << IPRemote << ":" << PortRemote; 
  Connected = true; 
  }

  qDebug() << "TRY TO CONNECT TO : " << IPRemote << ":" << PortRemote; 
  QTimer::singleShot(2000, this, SLOT(SlotCheckRequest()));
}

void UDPEngineInterface::SlotCheckConnection()
{

}

void UDPEngineInterface::SendCloseConnectionCommand()
{

}

bool UDPEngineInterface::isMessageAvailable() { return RingBuffer->isMessageAvailable(); }
