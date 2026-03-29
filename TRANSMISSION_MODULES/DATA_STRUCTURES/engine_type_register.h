#ifndef TYPE_REGISTER_ENGINE_H
#define TYPE_REGISTER_ENGINE_H
#include <QString>
#include <QDebug>
#include <typeinfo>
#include <string>
#include <map>

template<typename T = void*> 
class TypeRegister
{

public:
      static int  GetTypeID() { return TYPE_ID;}
      static int  GetTypeMax() { return TypeRegister<>::TypeIDMax;}
      static int  GetTypeCount() { return TypeCount;}
      static QString GetTypeNameStatic()  { return typeid(T).name(); }
      static QString GetTypeName()  {return QString::fromStdString((*TypeRegister<void*>::types)[TYPE_ID]); }
      static QString GetTypeSignature()  { return QString("REGISTER: %1 %2 SIZE: %3").arg(TYPE_ID)
                                                                                     .arg(GetTypeName()) 
                                                                                     .arg(GetTypeSize()); }

      static void printRegisteredTypes()
      {
      for(auto type: (*TypeRegister<void*>::types)) qDebug() << "[ TYPE REGISTERED ]" << type.first << type.second;
                                                    qDebug() << Qt::endl;
      }

      static bool isTypeRegistered()              { return ID() > 0;}
      static bool isTypeValid(const int& TYPE_ID) { return TYPE_ID <= TypeRegister<>::TypeIDMax;}

      constexpr static int ID() { return 0; };

      static constinit const int TYPE_ID;

      static int RegisterType(std::string name)  
      { 
            if(TypeRegister<void*>::types == nullptr) TypeRegister<void*>::types = new std::map<int,std::string>;
                                                     (*TypeRegister<void*>::types)[TYPE_ID] = name;
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
      static std::map<int,std::string>* types;
};

template<typename T> std::map<int,std::string>* TypeRegister<T>::types = nullptr;
template<typename T> int TypeRegister<T>::TypeCount = 0;
template<typename T> int TypeRegister<T>::TypeIDMax = 0;
template<typename T> int TypeRegister<T>::TypeSizeMin = 60000;
template<typename T> int TypeRegister<T>::TypeSizeMax = 0;
template<typename T> constinit const int TypeRegister<T>::TYPE_ID{TypeRegister<T>::ID()} ;

#endif 