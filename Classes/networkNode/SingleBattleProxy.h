/*
*   ����ģ�Ȿ��ս��������ͨ��
*   1.������Ҳ�������ֱ���ڱ��ش���ת����ս����
*
*/
#ifndef __SINGLEBATTLEPROXY_H__
#define __SINGLEBATTLEPROXY_H__

#include "EventProxy.h"

class CSingleBattleProxy : public CEventProxy
{
public:
    CSingleBattleProxy();
    virtual ~CSingleBattleProxy();

public:
    //��ʼ����������
    virtual bool init(CEventManager<int>* eventMgr);
    //����������ݴ���
    virtual int onRecv(char *buffer, int len);
    //���������ݴ���
    virtual int send(char *buffer, int len);
    //������
    virtual int onError(int error, int tag = 0, char *data = 0);
    //�ڲ�ֱ�Ӵ���ת��
    void onEventSend(void *data);
};

#endif 
