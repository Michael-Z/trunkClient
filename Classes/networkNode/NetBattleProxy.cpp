#include "NetBattleProxy.h"
#include "Protocol.h"
#include "PvpProtocol.h"
#include "BattleProtocol.h"
#include "KxCSComm.h"
#include "GameNetworkNode.h"

CNetBattleProxy::CNetBattleProxy()
{
}

CNetBattleProxy::~CNetBattleProxy()
{
}

bool CNetBattleProxy::init(CEventManager<int>* eventMgr)
{
    if (NULL == eventMgr)
    {
        return false;
    }

    CEventProxy::init(eventMgr);
    // ע���¼�id����Ҳ�����������Щ�¼�
    for (int i = CMD_BAT_CSBEGIN + 1; i < CMD_BAT_CSEND; i++)
    {
        m_pEventManager->addEventHandle(MakeCommand(CMD_BATTLE, i),
            this, CALLBACK_FUNCV(CNetBattleProxy::onEventSend));
    }

    return true;
}

int CNetBattleProxy::onRecv(char *buffer, int len)
{
    if (NULL != m_pEventManager)
    {
        Head *head = reinterpret_cast<Head*>(buffer);
        LOG("CNetBattleProxy OnRecv main %d sub %d", head->MainCommand(), head->SubCommand());
        m_pEventManager->raiseEvent(head->cmd, buffer);
    }

    return 0;
}

int CNetBattleProxy::send(char *buffer, int len)
{
    return CGameNetworkNode::getInstance()->sendData(buffer, len);
}

int CNetBattleProxy::onError(int error, int tag, char *data)
{
    //���ӳ���, ������ʾ��ϢRaise?
    return 0;
}

void CNetBattleProxy::onEventSend(void *data)
{
    //����������
    Head *head = reinterpret_cast<Head*>(data);
    int len = head->length;
    send(reinterpret_cast<char*>(data), len);
}
