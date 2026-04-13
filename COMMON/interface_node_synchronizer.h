#pragma once
#include "interface_pass_coord.h"
#include "interface_pass_value.h"
#include <QObject>

class NodeParamSynchronizer: public QObject
{
    Q_OBJECT
    public: 
    explicit NodeParamSynchronizer(PassValueClass<float>* Node, QObject* parent = nullptr): QObject(parent) 
    { NodeState = Node;  } 
    PassValueClass<float>* NodeState = nullptr;

    void linkPeers(NodeParamSynchronizer* DstNode) 
    { 
         this->connectSignal(DstNode);
      DstNode->connectSignal(this);
    };
    void connectSignal(NodeParamSynchronizer* DstNode) 
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

class NodeStateSynchronizer: public QObject
{
    Q_OBJECT
    public: 
    explicit NodeStateSynchronizer(PassCoordClass<float>* Node, QObject* parent = nullptr): QObject(parent) 
    { 
      NodeState = Node; 
    } 
    PassCoordClass<float>* NodeState = nullptr;

    std::pair<float,float> StatePass{0,0}; 
    void linkPeers(NodeStateSynchronizer* DstNode) 
    { 
         this->connectSignal(DstNode);
      DstNode->connectSignal(this);
    };

    void connectSignal(NodeStateSynchronizer* DstNode) 
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
