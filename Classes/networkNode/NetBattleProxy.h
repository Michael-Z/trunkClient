/*
*   ����ʵ��PVPս��������ͨ��
*   1.������Ҳ�������ת���������
*   2.���շ���˷�����������ҵĲ�����Ӧ�Լ�����ָ���������Ϣ֪ͨս����
*   
*/
#ifndef __NETBATTLEPROXY_H__
#define __NETBATTLEPROXY_H__

#include "EventProxy.h"

class CNetBattleProxy : public CEventProxy
{
public:
    CNetBattleProxy();
    virtual ~CNetBattleProxy();

public:
    //��ʼ��
    virtual bool init(CEventManager<int>* eventMgr);
    //����������ݴ���
    virtual int onRecv(char *buffer, int len);
    //���������ݴ���
    virtual int send(char *buffer, int len);
    //�������
    virtual int onError(int error, int tag = 0, char *data = 0);
    //�¼��ص��ӿ�
    void onEventSend(void *data);
};

#endif 
