#ifndef TYPE_REGISTER_ENGINE_H
#define TYPE_REGISTER_ENGINE_H
#include <QString>
#include <QDebug>
#include <typeinfo>

template<typename T = void*> 
class TypeRegister
{

public:
      static int  GetTypeID() { return TYPE_ID;}
      static int  GetTypeMax() { return TypeRegister<>::TypeIDMax;}
      static int  GetTypeCount() { return TypeCount;}
      static QString GetTypeName()  { return typeid(T).name(); }

      static bool isTypeRegistered()         { return ID() > 0;}
      static bool isTypeValid(const int& TYPE_ID) { return TYPE_ID <= TypeRegister<>::TypeIDMax;}

      constexpr static int ID() { return 0; };

      static constinit const int TYPE_ID;

      static int RegisterType()  
      { 
            TypeRegister<>::TypeCount++; 
            TypeRegister<>::TypeIDMax = (TYPE_ID > TypeRegister<>::TypeIDMax) ? TYPE_ID : TypeRegister<>::TypeIDMax;
            TypeRegister<>::SetTypeSize(sizeof(T));
            return TYPE_ID;
      }

      static int GetTypeSize()    {return sizeof(T);}
      static int GetMinTypeSize() {return TypeSizeMin;}
      static int GetMaxTypeSize() {return TypeSizeMax;}

      static void SetTypeSize(int size) 
      { 
            if(TypeSizeMin > size) TypeSizeMin = size; 
            if(TypeSizeMax < size) TypeSizeMax = size; 
      };

      static int TypeCount;
      static int TypeIDMax;
      static int TypeSizeMin;
      static int TypeSizeMax;
};

template<typename T> int TypeRegister<T>::TypeCount = 0;
template<typename T> int TypeRegister<T>::TypeIDMax = 0;
template<typename T> int TypeRegister<T>::TypeSizeMin = 60000;
template<typename T> int TypeRegister<T>::TypeSizeMax = 0;
template<typename T> constinit const int TypeRegister<T>::TYPE_ID{TypeRegister<T>::ID()} ;


#endif //TYPE_REGISTER_ENGINE_H