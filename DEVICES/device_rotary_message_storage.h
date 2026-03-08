#ifndef ROTARY_PLATFORM_MESSAGE_BUFFER_GENERIC_H
#define ROTARY_PLATFORM_MESSAGE_BUFFER_GENERIC_H

#include "message_command_structures.h"
//#include "engine_type_register_manual.h"
#include <cstring>
#include <QByteArray>
#include "message_storage_interface.h"
#include "queue"

class MessageStorageRotaryPlatform : public MessageStorageInterface
{
    public:
    MessageStorageRotaryPlatform();
    ~MessageStorageRotaryPlatform();

    QByteArray array;

    std::size_t  BufferSize = 500;
    std::queue<SystemState> MesageQueue1;
    std::queue<ControlRX>   MesageQueue2;

    bool isMessageAvailable() { return !MessageQueue1 | !MessageQueue2; };
    void appendData(uint8_t* Data, uint16_t Size) {};
    void reset();

    void* takeAnyMessage() override;

    int getMessagesInStore()  override { return MESSAGE_COUNTER;}

    int getDataInStore() override { return DATA_AVAILABLE; };
    int getDataPassed()  override { return DATA_PASSED; };
    constexpr int getMinMessageSize() override { return 10; } ; 

    int MESSAGE_COUNTER = 0;
    int MAX_MESSAGE_IN_STORE = M_N-2;

    int MAX_MESSAGE_SIZE = M_S;
    int MIN_MESSAGE_SIZE = M_S;

    int DATA_PASSED = 0;
    int DATA_AVAILABLE = 0;

    void linkTo(std::shared_ptr<MessageDispatcherInterface> Dispatcher) { LinkedDispatcher = Dispatcher; };
                std::shared_ptr<MessageDispatcherInterface> LinkedDispatcher = nullptr;
};



#endif //RING_BUFFER_GENERIC_H

