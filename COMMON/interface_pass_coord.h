#pragma once
#include <QPair>
#include <memory>
#include <vector>
#include <functional>
#include <QDebug>

template<typename V>
class PassCoordClass
{
public:
	std::vector<PassCoordClass*> NodesLinked ;
	bool isLinked() { return !NodesLinked.empty();}
    QPair<V,V> OutputCoord{0,0};
    bool PassBlocked = false;

	virtual const QPair<V, V>& GetOutput() { return OutputCoord;};
	virtual void SetInput(const QPair<V, V>& Coord) {OutputCoord = Coord;};

    void SetLink(PassCoordClass<V>* NewLink) { NodesLinked.push_back(NewLink); }
	void PassCoord() { if(!isLinked() || PassBlocked) return; for(auto& link: NodesLinked) {*this >> *link;} }
	
	virtual QPair<V,V>& operator >>(QPair<V, V>& Coord) { Coord = GetOutput(); return Coord;}

    virtual PassCoordClass& operator >>(PassCoordClass& Reciever)
    { 
        Reciever.SetInput(GetOutput()); if(Reciever.isLinked()) Reciever.PassCoord(); return Reciever; 
    }

	friend PassCoordClass& operator >>(const     QPair<V,V>&  Coord, PassCoordClass& Reciever)
    { Reciever.SetInput(Coord); if(Reciever.isLinked()) Reciever.PassCoord(); return Reciever; } 

    friend PassCoordClass& operator | (PassCoordClass& Sender, PassCoordClass& Reciever)
    { Sender.SetLink(&Reciever); return Reciever; }

    friend std::shared_ptr<PassCoordClass>  operator | (std::shared_ptr<PassCoordClass> Sender, 
	                                                    std::shared_ptr<PassCoordClass> Reciever)
    { Sender->SetLink(Reciever.get()); return Reciever; }

    friend PassCoordClass& operator | (std::shared_ptr<PassCoordClass> Sender, PassCoordClass& Reciever)
    { Sender->SetLink(&Reciever); return Reciever; }

};


template<typename T>
QPair<T, T> operator*(QPair<T, T> x, const QPair<T, T>& y) { x.first *= y.first; x.second *= y.second; return x;}

template<typename T, typename V>
QPair<V, V> operator*(QPair<V, V> x, const QPair<T, T>& y) { x.first *= y.first; x.second *= y.second; return x;}

template<typename T> QPair<T, T> operator*(QPair<T, T> x, const T& Scale)   { x.first *= Scale; x.second *= Scale ; return x; }
template<typename T> QPair<T, T> operator+(QPair<T, T> x, const T& addition){ x.first += addition; x.second += addition;  return x; }

template<typename T> QPair<T, T> operator/(QPair<T, T> x, const T& Scale)   { x.first /= Scale; x.second /= Scale ; return x; }

template<typename T>
QPair<T, T> operator/(QPair<T, T> x, const QPair<T, T>& y) { x.first /= y.first; x.second /= y.second; return x;}
template<typename T, typename V>
QPair<V, V> operator/(QPair<V, V> x, const QPair<T, T>& y) { x.first /= y.first; x.second /= y.second; return x;}

template<typename T>
std::pair<T, T> operator+(std::pair<T, T> x, const std::pair<T, T>& y) { x.first += y.first; x.second += y.second; return x; }

template<typename T>
std::pair<float,float> operator+(std::pair<float,float> x, const std::pair<T, T>& y) { x.first += y.first; x.second += y.second; return x; }
template<typename T>
std::pair<float,float> operator+(const std::pair<T, T>& x, std::pair<float,float> y) { y.first += x.first; y.second += x.second; return y; }

template<typename T>
std::pair<T, T> operator-(std::pair<T, T> x, const std::pair<T, T>& y) { x.first -= y.first; x.second -= y.second; return x; }

template<typename T, typename V>
std::pair<T, T> operator-(std::pair<T, T> x, const std::pair<V, V>& y) { x.first -= y.first; x.second -= y.second; return x; }

template<typename T>
std::pair<T, T> abs_pair(std::pair<T, T> x) 
{ 
    x.first  = std::abs(x.first); 
    x.second = std::abs(x.second); 
    return x;
}


template <class T>
class PortAdapter : public PassCoordClass<float>
{
public:
    PortAdapter(){};
    void LinkAdapter(T* LinkObject,std::function<void(T&,QPair<float,float>)> SetFunction, std::function<QPair<float,float>(T&)> GetFunction)
    { InputFunction = SetFunction; OutputFunction = GetFunction; Receiver = LinkObject; };

    T* Receiver =  nullptr;

    std::function<void(T&,QPair<float,float>)> InputFunction = nullptr;
    std::function<QPair<float,float>(T&)> OutputFunction = nullptr;

    void SetInput(const QPair<float,float>& coord) { if(InputFunction != nullptr) InputFunction(*Receiver,coord);};
    const QPair<float,float>& GetOutput() { if(OutputFunction != nullptr) PassCoordClass<float>::OutputCoord = OutputFunction(*Receiver); 
                                                                   return PassCoordClass<float>::OutputCoord;};

};