#pragma once
#include "interface_pass_coord.h"
#include "interface_pass_value.h"
#include "device_generic_interface.h"
#include <QObject>

class NodeSignalAdapter: public QObject
{
    Q_OBJECT
    public: 
    explicit NodeSignalAdapter(DeviceGenericHandleControl* Node, uint32_t SignalNumber = 0, QObject* parent = nullptr): QObject(parent) 
    { NodeState = Node; Number = SignalNumber; } 
    DeviceGenericHandleControl* NodeState = nullptr;
    uint32_t Number = 0;

        void operator|(NodeSignalAdapter* DstNode) { linkPeers(DstNode); }
    void connectSignal(NodeSignalAdapter* DstNode) { QObject::connect(this, SIGNAL(signalStateChanged()), DstNode, SLOT(slotSetState(float))); };
        void linkPeers(NodeSignalAdapter* DstNode) 
        { 
            this->connectSignal(DstNode);
            DstNode->connectSignal(this); };

    friend void operator>>(bool signal, NodeSignalAdapter& Node) { if(signal) Node.emitSignal(); } ;
                                                                              void emitSignal() { emit signalStateChanged(); }

    public slots:
    void slotSetSignal() { NodeState->setEnable(true,Number);}
    signals:
    void signalStateChanged();
};

class NodeParamSignalAdapter: public QObject
{
    Q_OBJECT
    public: 
    explicit NodeParamSignalAdapter(PassValueClass<float>* Node, QObject* parent = nullptr): QObject(parent) 
    { NodeState = Node;  } 
    PassValueClass<float>* NodeState = nullptr;

    void linkPeers(NodeParamSignalAdapter* DstNode) 
    { 
         this->connectSignal(DstNode);
      DstNode->connectSignal(this);
    };
    void connectSignal(NodeParamSignalAdapter* DstNode) 
    { 
      QObject::connect(this, SIGNAL(signalStateChanged(float)), DstNode, SLOT(slotSetState(float)));
    };
    void synchronizePeers() 
    { 
      emit signalStateChanged(NodeState->getValue());
    }
    public slots:
    void slotSetState(float param) { param >> *NodeState;}
    signals:
    void signalStateChanged(float param);
};

class NodeCoordSignalAdapter: public QObject
{
    Q_OBJECT
    public: 
    explicit NodeCoordSignalAdapter(PassCoordClass<float>* Node, QObject* parent = nullptr): QObject(parent) 
    { 
      NodeState = Node; 
    } 
    PassCoordClass<float>* NodeState = nullptr;

    std::pair<float,float> StatePass{0,0}; 
    void linkPeers(NodeCoordSignalAdapter* DstNode) 
    { 
         this->connectSignal(DstNode);
      DstNode->connectSignal(this);
    };

    void connectSignal(NodeCoordSignalAdapter* DstNode) 
    { 
      QObject::connect(this, SIGNAL(signalStateChanged(std::pair<float,float>)), DstNode, SLOT(slotSetState(std::pair<float,float>)));
    };
    void synchronizePeers() 
    { 
                              StatePass = NodeState->getOutput();
      emit signalStateChanged(StatePass);
      qDebug() << "[ SYNCHRONIZER PASS ]" << StatePass.first << StatePass.second;
    }
    public slots:
    void slotSetState(std::pair<float,float> State) {StatePass = State; StatePass >> *NodeState; }
    signals:
    void signalStateChanged(std::pair<float,float>);
};

template<typename T, typename M> void linkPeers(T* src, M* dst) { src->NodeSynchronizer.linkPeers(&dst->NodeSynchronizer); }
template<typename T, typename M> void synchronizePeers(T* src, M* dst) { src->NodeSynchronizer.linkPeers(&dst->NodeSynchronizer); }
