#ifndef MESSAGEITERATOR_COMMON_H
#define MESSAGEITERATOR_COMMON_H

//#include <utility>
//#include <cstring>
//#include <queue>

#include  <cstdint>
#include <QDebug>
#include <QBuffer>

#include <message_struct_generic_ext.h>

class QBufferEx : public QBuffer
{
    public:
    QBufferEx() {};
    QBufferEx(QByteArray* array): QBuffer(array) { };
    const QByteArray constData(int size)
    {
        return QByteArray::fromRawData(this->buffer().constData() + this->pos(), size);
    }
    const QByteArray constData()
    {
        return QByteArray::fromRawData(this->buffer().constData() + this->pos(), this->buffer().size() - this->pos());
    }
};

template<typename H>
class MessageIteratorGenericApp {

public:
    MessageIteratorGenericApp();
    MessageIteratorGenericApp(const MessageIteratorGenericApp<H>& Message);
    MessageIteratorGenericApp(QByteArray& STORAGE);
    ~MessageIteratorGenericApp();
    QByteArray* Buffer;

    QBufferEx MessageBegin;    
    QBufferEx MessageDataBegin;    
    QBufferEx MessageNextBegin;
    QBuffer WritePointer; 

	uint32_t WaitDataAmount;
    uint32_t MessageNumber = 0;

const MessageGenericExt<void*,H> & operator*();
const H& GetHeader();

           char* DataPtr()   { return (char*)MessageBegin.constData().data();};
const QByteArray DataArray() { return MessageBegin.constData();         }

MessageIteratorGenericApp<H>& operator++(int);
MessageIteratorGenericApp<H>& operator++();

void operator=(const MessageIteratorGenericApp<H>& Message);

bool operator==(MessageIteratorGenericApp<H>& Message);
bool operator!=(MessageIteratorGenericApp<H>& Message);

void LoadData(uint8_t* DataSourceBuffer, uint8_t DataAvailable);
void LoadData(QByteArray Data);
MessageIteratorGenericApp<H>& SwitchToNext();

    int CountMemoryAvailable() { return WritePointer.data().capacity() - WritePointer.pos(); }
    int MAX_MESSAGE_SIZE = 50;

private:
    bool IsMessageFull();
	bool IsHeaderValid();
	bool IsMemoryAtEnd();
	int  DataWait();
	virtual void ParseMessage();
};

template<typename H> MessageIteratorGenericApp<H>& MessageIteratorGenericApp<H>::operator++() { return SwitchToNext(); }
template<typename H> MessageIteratorGenericApp<H>& MessageIteratorGenericApp<H>::operator++(int) { return SwitchToNext(); }
template<typename H>
MessageIteratorGenericApp<H>& MessageIteratorGenericApp<H>::SwitchToNext()
{
if( IsMemoryAtEnd() || !GetHeader().isValid()) MessageBegin.seek(0);
MessageNumber++;
//qDebug() << "SWITCH TO NEXT POS: " << MessageBegin.pos() << " BYTE STEP: " << (sizeof(H) + GetHeader().DATA_SIZE);
MessageBegin.seek(MessageBegin.pos() + sizeof(H) + GetHeader().DATA_SIZE);
MessageDataBegin.seek(MessageDataBegin.pos() + sizeof(H) + GetHeader().DATA_SIZE);
return *this;
}


template<typename H>
MessageIteratorGenericApp<H>::MessageIteratorGenericApp(const MessageIteratorGenericApp<H>& Message)
{
    Buffer = Message.Buffer;
    if(!MessageBegin.isOpen())
    {
        MessageBegin.setBuffer(Buffer); MessageBegin.open(QIODevice::ReadWrite);
    MessageDataBegin.setBuffer(Buffer); MessageDataBegin.open(QIODevice::ReadWrite);
    MessageNextBegin.setBuffer(Buffer); MessageNextBegin.open(QIODevice::ReadWrite);
        WritePointer.setBuffer(Buffer); WritePointer.open(QIODevice::ReadWrite);
        
    }
    WritePointer.seek(Message.WritePointer.pos());
    MessageBegin.seek(Message.MessageBegin.pos()); 
    MessageNextBegin.seek(Message.MessageNextBegin.pos()); 
    MessageDataBegin.seek(Message.MessageDataBegin.pos());
    WaitDataAmount = Message.WaitDataAmount;
    //qDebug() << "COPY CONSTR: BUFFER CAPACITY: " << WritePointer.buffer().capacity() << "PTR: " << (uint8_t*)WritePointer.buffer().data();
}

template<typename H> MessageIteratorGenericApp<H>::MessageIteratorGenericApp() { }

template<typename H>
MessageIteratorGenericApp<H>::~MessageIteratorGenericApp()
{
//qDebug() << "DELET MESSAGE_ITERATOR_COMMON";
MessageBegin.close();
WritePointer.close();
MessageNextBegin.close();
}


template<typename H>
void MessageIteratorGenericApp<H>::LoadData(uint8_t* DataSourceBuffer, uint8_t DataAvailable)
{
             if(DataAvailable == 0) return;
          auto WaitDataAmount = DataWait();
             if(WaitDataAmount > DataAvailable) WaitDataAmount = DataAvailable;

WritePointer.write((const char*)DataSourceBuffer,WaitDataAmount);
ParseMessage(); 

LoadData(DataSourceBuffer + WaitDataAmount, DataAvailable - WaitDataAmount); //CONTINUE RECURSIVE LOAD DATA IF AVAILABLE
}

template<typename H>
void MessageIteratorGenericApp<H>::LoadData(QByteArray Data)
{
             if(Data.isEmpty()) return;
           auto WaitDataAmount = DataWait();
             if(WaitDataAmount > Data.size()) WaitDataAmount = Data.size();

auto DataRead = Data; DataRead.truncate(WaitDataAmount); 

WritePointer.write(DataRead);
ParseMessage(); 

Data = Data.remove(0,WaitDataAmount); LoadData(Data); //CONTINUE RECURSIVE LOAD DATA IF AVAILABLE
}


template<typename H>
void MessageIteratorGenericApp<H>::ParseMessage()
{
 if(!IsHeaderValid()) return; 
 if(!IsMessageFull()) return; 

 MessageNextBegin.seek(MessageBegin.pos() + sizeof(H) + GetHeader().DATA_SIZE); //USED ONLY FOR CONVINIENCE WHEN DATA GET
 MessageDataBegin.seek(MessageBegin.pos() + sizeof(H)); 

     MessageBegin.seek(MessageNextBegin.pos()); //SWITCH TO NEXT MESSAGE
     MessageNumber++;

 if( !IsMemoryAtEnd()) return;
   MessageBegin.seek(0);
   WritePointer.seek(0);
}

template<typename H>
int  MessageIteratorGenericApp<H>::DataWait() //IF HEADER NOT LOADED WAIT sizeof(HEADER), else WAIT DATA_SIZE
{
  WaitDataAmount = sizeof(H); //IF NO HEADER WAIT HEADER

  auto& HEADER = GetHeader(); //IF GOT HEADER  WAIT DATA
     if(HEADER.isValid()) WaitDataAmount = HEADER.DATA_SIZE + (uint32_t)sizeof(H) - (WritePointer.pos() - MessageBegin.pos());
  
  return WaitDataAmount;
}

template<typename H> bool MessageIteratorGenericApp<H>::operator!=(MessageIteratorGenericApp& Message)    { return !(Message == *this); }
template<typename H> bool MessageIteratorGenericApp<H>::operator==(MessageIteratorGenericApp<H>& Message) { return MessageBegin.pos() == Message.MessageBegin.pos(); }

template<typename H> bool MessageIteratorGenericApp<H>::IsMessageFull() { if(DataWait() == 0) return true; return false; }
template<typename H> bool MessageIteratorGenericApp<H>::IsHeaderValid() { return GetHeader().isValid(); }
template<typename H> bool MessageIteratorGenericApp<H>::IsMemoryAtEnd() { return WritePointer.data().capacity() - WritePointer.pos() < (MAX_MESSAGE_SIZE + sizeof(H));     }

template<typename H> const H& MessageIteratorGenericApp<H>::GetHeader() { return *reinterpret_cast<const H*>(MessageBegin.constData().data()); }
template<typename H> const MessageGenericExt<void*, H>& MessageIteratorGenericApp<H>::operator*() 
{ 
    
    auto& GenericMessage = *reinterpret_cast<const MessageGenericExt<void*, H>*>(MessageBegin.constData().data()); 
          //GenericMessage.DATA = &GenericMessage.DATA;
          return GenericMessage;
}

template<typename H>
void MessageIteratorGenericApp<H>::operator=(const MessageIteratorGenericApp<H>& Message) 
{ 
    //if(MessageBegin.data().data() != MessageBegin)
    Buffer = Message.Buffer;
    if(!MessageBegin.isOpen())
    {
        MessageBegin.setBuffer(Buffer); MessageBegin.open(QIODevice::ReadWrite);
    MessageDataBegin.setBuffer(Buffer); MessageDataBegin.open(QIODevice::ReadWrite);
    MessageNextBegin.setBuffer(Buffer); MessageNextBegin.open(QIODevice::ReadWrite);
        WritePointer.setBuffer(Buffer); WritePointer.open(QIODevice::ReadWrite);
        
    }
    WritePointer.seek(Message.WritePointer.pos());
    MessageBegin.seek(Message.MessageBegin.pos()); 
    MessageNextBegin.seek(Message.MessageNextBegin.pos()); 
    MessageDataBegin.seek(Message.MessageDataBegin.pos());
    WaitDataAmount = Message.WaitDataAmount;
    //qDebug() << "COPY ITERATOR: BUFFER CAPACITY: " << WritePointer.buffer().capacity() << "PTR: " << (uint8_t*)WritePointer.buffer().data();
}

template<typename H>
MessageIteratorGenericApp<H>::MessageIteratorGenericApp(QByteArray& STORAGE)
{
   //qDebug() << "CREATE MESSAGE ITERATOR TO BUFFER: " << STORAGE.capacity();
   MessageBegin.setBuffer(&STORAGE); MessageBegin.open(QIODevice::ReadWrite);
   MessageDataBegin.setBuffer(&STORAGE); MessageDataBegin.open(QIODevice::ReadWrite);
   MessageNextBegin.setBuffer(&STORAGE); MessageNextBegin.open(QIODevice::ReadWrite);
   WritePointer.setBuffer(&STORAGE); WritePointer.open(QIODevice::ReadWrite);

   MessageBegin.seek(0);
   MessageNextBegin.seek(0);
   MessageDataBegin.seek(0);
   WritePointer.seek(0);
   Buffer = &STORAGE;
}

#endif //MESSAGEITERATOR_COMMON_H
