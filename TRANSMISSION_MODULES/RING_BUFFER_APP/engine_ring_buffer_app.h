#pragma once
#include <QByteArray>
//#include <cstring>
#include "message_command_structures.h"
#include "message_iterator_generic_app.h"
#include "message_struct_generic_ext.h"
#include "engine_type_register.h"


class RingBufferInterfaceApp
{
    public:
virtual void AppendData(QByteArray Data) = 0;
virtual void SkipMessages(int Number) = 0;
virtual bool isMessageAvailable() = 0;

virtual  int CountMemoryAvailable() = 0;
         int CountMessageInStore() { return MESSAGE_COUNTER; };

                   virtual const QByteArray& TakeMessageData() = 0;
template<typename H> MessageGenericExt<void*,H>* TakeMessagePtr();
template<typename H> MessageGenericExt<void*,H>& TakeMessage();

        int MAX_FRAME_SIZE = 40;
        int MESSAGE_COUNTER = 0;

QByteArray InputDataBuffer; //RING BUFFER OF FIXED SIZE WHERE DATA PUT BY LastDataPointer
};


template<typename H, size_t M_S, size_t M_N>
class EngineRingBufferApp : public RingBufferInterfaceApp
{
    public:
    EngineRingBufferApp();
    ~EngineRingBufferApp();

  void AppendData(QByteArray Data);
  void AppendData(uint8_t* Data, uint8_t DataToRead);
  bool isMessageAvailable();

  virtual                    void SkipMessages(int Number);
  virtual MessageGenericExt<void*,H>* TakeMessagePtr();

  virtual MessageGenericExt<void*,H>& TakeMessage();
                const QByteArray& TakeMessageData();
  int CountMemoryAvailable() { return IncommingMessagePointer.CountMemoryAvailable();};

  MessageIteratorGenericApp<H> AvailableMessagePointer;
  MessageIteratorGenericApp<H> IncommingMessagePointer;
  MessageIteratorGenericApp<H> begin() { return AvailableMessagePointer;}
  MessageIteratorGenericApp<H> end() { return IncommingMessagePointer;};

  size_t BUFFER_CAPACITY = M_N;
  size_t MAX_MESSAGE_SIZE = M_S;
  size_t MIN_MESSAGE_SIZE = M_S/2;

  template<typename T> void RegisterMessage(T MESSAGE);

};

template<typename H, size_t M_S, size_t M_N>
template<typename T>
void EngineRingBufferApp<H,M_S,M_N>::RegisterMessage(T MESSAGE)
{
   int MessageSize = MessageGenericExt<T,H>().toByteArray().size();
    if(MessageSize < MIN_MESSAGE_SIZE) MIN_MESSAGE_SIZE = MessageSize;

TypeRegister<T>::RegisterType();

qDebug() << "REGISTER MESSAGE SIZE: " << MessageSize << "MIN SIZE: " << MIN_MESSAGE_SIZE;
}


template<typename H, size_t M_S, size_t M_N>
EngineRingBufferApp<H,M_S,M_N>::EngineRingBufferApp()
{
     InputDataBuffer.reserve(BUFFER_CAPACITY*MAX_FRAME_SIZE);
     AvailableMessagePointer = MessageIteratorGenericApp<H>(InputDataBuffer);
     IncommingMessagePointer = MessageIteratorGenericApp<H>(InputDataBuffer);

     qDebug() << "[ CREATE RING BUFFER ENGINE ]";
     qDebug() << "INPUT BUFFER SIZE: " << InputDataBuffer.capacity() 
              << "MIN MESSAGE SIZE: " << MIN_MESSAGE_SIZE
              << "MAX MESSAGE SIZE: " << MAX_MESSAGE_SIZE;

	MAX_MESSAGE_SIZE = TypeRegister<>::GetMaxTypeSize() + sizeof(H);
	MIN_MESSAGE_SIZE = TypeRegister<>::GetMinTypeSize() + sizeof(H);
	qDebug() << "RING BUFFER DENSE MESSAGE SIZE RANGE: "<< MIN_MESSAGE_SIZE << MAX_MESSAGE_SIZE;
}

template<typename H, size_t M_S, size_t M_N>
void EngineRingBufferApp<H,M_S,M_N>::AppendData(QByteArray Data)
{
    IncommingMessagePointer.LoadData(Data);
    MESSAGE_COUNTER = IncommingMessagePointer.MessageNumber - AvailableMessagePointer.MessageNumber;

    if(MESSAGE_COUNTER > 8) AvailableMessagePointer++;
    qDebug() << "MESSAGE AVAILABLE: " << MESSAGE_COUNTER 
             << "PASS: " << AvailableMessagePointer.MessageNumber 
             << "AVAILABLE: " << isMessageAvailable();

}

template<typename H, size_t M_S, size_t M_N>
void EngineRingBufferApp<H,M_S,M_N>::AppendData(uint8_t* Data, uint8_t DataToRead)
{
    IncommingMessagePointer.LoadData(Data,DataToRead);
    MESSAGE_COUNTER = IncommingMessagePointer.MessageNumber - AvailableMessagePointer.MessageNumber;

    if(MESSAGE_COUNTER > 8) AvailableMessagePointer++;
    qDebug() << "MESSAGE AVAILABLE: " << MESSAGE_COUNTER 
             << "PASS: " << AvailableMessagePointer.MessageNumber 
             << "AVAILABLE: " << isMessageAvailable();

}

template<typename H, size_t M_S, size_t M_N>
bool EngineRingBufferApp<H,M_S,M_N>::isMessageAvailable() 
{ 
    return AvailableMessagePointer != IncommingMessagePointer; 
}

template<typename H, size_t M_S, size_t M_N>
const QByteArray& EngineRingBufferApp<H,M_S,M_N>::TakeMessageData() { return AvailableMessagePointer.DataArray(); }

template<typename H, size_t M_S, size_t M_N>
MessageGenericExt<void*,H>* EngineRingBufferApp<H,M_S,M_N>::TakeMessagePtr()
{
    auto CurrentMessage = reinterpret_cast<MessageGenericExt<void*,H>*>(AvailableMessagePointer.DataPtr()); 
    if(!isMessageAvailable()) return CurrentMessage;

    AvailableMessagePointer++;
    MESSAGE_COUNTER = IncommingMessagePointer.MessageNumber - AvailableMessagePointer.MessageNumber;
    return CurrentMessage;
}

template<typename H, size_t M_S, size_t M_N>
MessageGenericExt<void*,H>& EngineRingBufferApp<H,M_S,M_N>::TakeMessage()
{
    auto& CurrentMessage = *reinterpret_cast<MessageGenericExt<void*,H>*>(AvailableMessagePointer.DataPtr()); 
    if(!isMessageAvailable()) return CurrentMessage;

    AvailableMessagePointer++;
    MESSAGE_COUNTER = IncommingMessagePointer.MessageNumber - AvailableMessagePointer.MessageNumber;
    return CurrentMessage;
}

template<typename H, size_t M_S, size_t M_N>
void EngineRingBufferApp<H,M_S,M_N>::SkipMessages(int Number) 
{ 
    while(Number != 0) { AvailableMessagePointer++; Number--; }
}

//=====================================================================
template<typename H, size_t M_S, size_t M_N>
EngineRingBufferApp<H,M_S,M_N>::~EngineRingBufferApp()
{
    qDebug() << "DELETE RING BUFFER";
}


//auto& MessageReturn = *reinterpret_cast<MessageGenericExt<ControlMessage,MESSAGE_HEADER>*>(RingBuffer.TakeMessage());
