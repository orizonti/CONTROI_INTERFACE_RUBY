#pragma once
#include <memory>
#include <vector>


template<typename V = float>
class PassValueClass
{
public:
	std::vector<PassValueClass*> NodesLinked;
	bool isLinked() { return !NodesLinked.empty();}
	V Value{0};
    bool enablePass = false;

	virtual const V& getValue() { return Value;};
	virtual void setValue(V InputValue) { Value = InputValue; };

    void setLink(PassValueClass* NewLink) { NodesLinked.push_back(NewLink);}
	void passValue() { if(!isLinked()) return; for(auto& Link: NodesLinked) *this >> *Link; }

	//==========================================================================================
    virtual PassValueClass<V>& operator >>(PassValueClass<V>& Reciever)
	{ Reciever.setValue(Value); if(Reciever.isLinked()) Reciever.passValue(); return Reciever; }

	virtual V& operator >>(V& Output) { Output = Value; return Output; }

	//==========================================================================================
	
	friend PassValueClass& operator >>(               V Value, PassValueClass& Reciever)
	{ Reciever.setValue(Value); if(Reciever.isLinked()) Reciever.passValue(); return Reciever; };

    friend PassValueClass& operator | (PassValueClass& Sender, PassValueClass& Reciever)
	{ Sender.setLink(&Reciever); return Reciever; }

    friend std::shared_ptr<PassValueClass >  operator | (std::shared_ptr<PassValueClass > Sender, 
														 std::shared_ptr<PassValueClass > Reciever)
	{ Sender->setLink(Reciever.get()); return Reciever; }

    friend PassValueClass& operator | (std::shared_ptr<PassValueClass > Sender, PassValueClass& Reciever)
	{ Sender->setLink(&Reciever); return Reciever; }





};

