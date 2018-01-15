#ifndef __COMMONPROXY_H__
#define __COMMONPROXY_H__

#include "EventProxy.h"
#include "BufferData.h"

class CCommonProxy : public CEventProxy
{
public:
    CCommonProxy();
    virtual ~CCommonProxy();

public:
    //��ʼ����������
    virtual bool init(CEventManager<int>* eventMgr);
    //����������ݴ���
    virtual int onRecv(char *buffer, int len);
    //���������ݴ���
    virtual int send(char *buffer, int len);
    //������
    virtual int onError(int error, int tag = 0, char *data = 0);

    //�¼��ص��ӿ�
    virtual void onEventSend(void *data);

private:
    CBufferData* m_BufferData;
};

#endif 
