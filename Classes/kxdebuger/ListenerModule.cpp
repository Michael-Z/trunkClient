#include "ListenerModule.h"
#include "KxDebuger.h"
#include "KxPollNode.h"

namespace kxdebuger {

ListenerModule::ListenerModule()
{
}


ListenerModule::~ListenerModule()
{
}

void ListenerModule::processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target)
{
    // ���ͻ�����ӵ�Poller��
    if (target)
    {
        KxDebuger::getInstance()->getPoller()->addCommObject(
            target, target->getPollType());
    }
}

void ListenerModule::processError(KxServer::IKxComm *target)
{

}

}