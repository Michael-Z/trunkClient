#include "SingleBattleProxy.h"
#include "BattleProtocol.h"
#include "Protocol.h"

CSingleBattleProxy::CSingleBattleProxy()
{
}

CSingleBattleProxy::~CSingleBattleProxy()
{
}

bool CSingleBattleProxy::init(CEventManager<int>* eventMgr)
{
    if (NULL == eventMgr)
    {
        return false;
    }

    CEventProxy::init(eventMgr);
    //ע���¼�id
    //battle
    m_pEventManager->addEventHandle(MakeCommand(CMD_BATTLE, CMD_BAT_PVPCOMMANDCS),
        this, CALLBACK_FUNCV(CSingleBattleProxy::onEventSend));
    return true;
}

int CSingleBattleProxy::onRecv(char *buffer, int len)
{
    if (NULL != m_pEventManager)
    {
        Head *head = reinterpret_cast<Head*>(buffer);
        m_pEventManager->raiseEvent(head->cmd, buffer);
    }
    
    return 0;
}

int CSingleBattleProxy::send(char *buffer, int len)
{
    //������Ϣ, ��Ҫ�м�������Ϣ����, ����ֱ��onRecv
    return 0;
}

int CSingleBattleProxy::onError(int error, int tag, char *data)
{
    //nothing to do
    return 0;
}

void CSingleBattleProxy::onEventSend(void *data)
{
    //����������
    Head *head = reinterpret_cast<Head*>(data);
    int len = head->length;

    // ת�ɷ�������Ӧ������
    int offset = head->SubCommand() - CMD_BAT_CSBEGIN;
    head->MakeCommand(CMD_BATTLE, CMD_BAT_SCBEGIN + offset);

    onRecv(reinterpret_cast<char*>(data), len);
}
