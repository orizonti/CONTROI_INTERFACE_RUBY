#ifndef TYPE_REGISTER_ENGINE_H
#define TYPE_REGISTER_ENGINE_H
#include <QString>
#include <QDebug>

template<typename T = void*> 
class TypeRegister
{

public:
      static uint8_t TYPE_ID;
      static int     GetTypeID()    { return TYPE_ID;}
      static size_t  GetTypeCount() { return TypeCount;}
      static QString Name;
      static QString GetTypeName()              { return Name; }
      static QString GetTypeName(const int& ID) { return Name; }

      static bool isTypeValid()              { return GetTypeID() > 0;}
      static bool isTypeValid(const int& ID) { return ID <= TypeRegister<>::TypeCount;}

      static int RegisterType(QString TypeName = "NONE")  
      { 
                if(TYPE_ID > 0) return TYPE_ID; // ALREADY REGISTERED
            Name = TypeName;
            TypeRegister<>::TypeCount++; TYPE_ID = TypeRegister<>::TypeCount;
            TypeRegister<>::SetTypeSize(sizeof(T));
            return TYPE_ID;
      }

      static size_t GetTypeSize()    {return sizeof(T);}
      static size_t GetMinTypeSize() {return TypeSizeMin;}
      static size_t GetMaxTypeSize() {return TypeSizeMax;}

      static void SetTypeSize(int size) 
      { 
            if(TypeSizeMin > size) TypeSizeMin = size; 
            if(TypeSizeMax < size) TypeSizeMax = size; 
      };

      static uint8_t TypeCount;
      static size_t TypeSizeMin;
      static size_t TypeSizeMax;
};


template<typename T> QString TypeRegister<T>::Name = "NONE";

template<typename T> uint8_t TypeRegister<T>::TypeCount = 0;
template<typename T> size_t TypeRegister<T>::TypeSizeMin = 60000;
template<typename T> size_t TypeRegister<T>::TypeSizeMax = 0;

template<typename T> constinit uint8_t TypeRegister<T>::TYPE_ID = 0;

#endif //TYPE_REGISTER_ENGINE_H