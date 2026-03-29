#ifndef MessageGeneric_H
#define MessageGeneric_H

#include <QDataStream>
#include <QIODevice>

#include <algorithm>
#include <set>
#include "engine_type_register.h"
#include <QByteArray>
#include <array>

template<class HEADER_TYPE>
concept HEADER_GENERIC = requires(HEADER_TYPE H) { H.getMessageSize(); H.isValid(); HEADER_TYPE::getHeaderMarker(); };

template<class HEADER_TYPE>
concept HEADER_VARIABLE = requires(HEADER_TYPE H) {
                                                   H.getMessageSize(); 
                                                   H.isValid(); 
                                                   HEADER_TYPE::getHeaderMarker(); 
                                                   HEADER_TYPE::getMaxSize();
                                                   HEADER_TYPE::getMaxSizeStatic();
                                                  };

template<class HEADER_TYPE>
concept HEADER_STANDART = requires(HEADER_TYPE H) { H.DATA_SIZE; H.getMessageSize(); H.isValid(); HEADER_TYPE::getHeaderMarker();};


//template<typename T, typename H> requires HEADER_GENERIC<H>

template<typename T, HEADER_GENERIC H> 
class MessageGeneric
{
  public:
    MessageGeneric() 
    { 
      HEADER.MESSAGE_IDENT = TypeRegister<T>::GetTypeID(); 
      //qDebug() << "[ CREATE MESSAGE GENERIC  NON STANDART ]";
    };
	public:
    H HEADER;
    T DATA; 
  public:
                  bool isMessasge()     { return (HEADER.isValid()); };
                   int GetSize()        { return HEADER.getMessageSize();  };
  constexpr static int GetSizeMessage() { return sizeof(MessageGeneric<T,H>); };
  MessageGeneric<void*,H>& toGenericMessage() { return *reinterpret_cast<MessageGeneric<void*,H>*>(this);  }
};

template<typename T, HEADER_VARIABLE H> 
class MessageGeneric<T, H>
{
  public:
    MessageGeneric()
    { 
      defaultMessage >> DATA;
      HEADER.MESSAGE_IDENT = TypeRegister<T>::GetTypeID(); 
      //qDebug() << "[ CREATE MESSAGE GENERIC VARIABLE ] SIZE: " << H::getMaxSizeStatic() 
      //         << "ID: " << TypeRegister<T>::GetTypeID()
      //         << "TYPE: " << TypeRegister<T>::GetTypeName();
    };
	public:
    static H HEADER;
    std::array<uint8_t,H::getMaxSizeStatic()> DATA;
  public:
	                bool isMessasge()     { return ((H*)(&DATA))->isValid(); };
                   int GetSize()        { return H::getMaxSize(); };
  constexpr static int GetSizeMessage() { return H::getMaxSize(); };

         void operator>>(T& Rec) { Rec.loadData(DATA); }
  friend void operator>>(T& Src, MessageGeneric<T,H>& Rec) { Src >> Rec.DATA; }

  MessageGeneric<void*,H>& toGenericMessage() { return *reinterpret_cast<MessageGeneric<void*,H>*>(this);  }
  private:
  static T defaultMessage;

};
template<typename T, HEADER_VARIABLE H> H MessageGeneric<T,H>::HEADER;
template<typename T, HEADER_VARIABLE H> T MessageGeneric<T,H>::defaultMessage;

template<typename T, HEADER_STANDART H> 
class MessageGeneric<T,H>
{
  public:
    MessageGeneric()
    { 
      HEADER.DATA_SIZE = sizeof(T);
      HEADER.MESSAGE_IDENT = TypeRegister<T>::GetTypeID();
      //qDebug() << "[ CREATE MESSAGE GENERIC STANDART ]";
    };
	public:
    H HEADER;
    T DATA; 
  public:

                  bool isMessasge() { return (HEADER.isValid()); };
                   int GetSize()       { return HEADER.DATA_SIZE + sizeof(H);  };
  constexpr static int GetSizeMessage() { return sizeof(MessageGeneric<T,H>); };

  MessageGeneric<void*,H>& toGenericMessage() { return *reinterpret_cast<MessageGeneric<void*,H>*>(this);  }

};

//==========================================================

#endif 