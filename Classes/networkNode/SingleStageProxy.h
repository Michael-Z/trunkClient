/*
*   ���ڱ��ص�������ؿ�
*   1.������ҽ���ؿ�������
*   2.ģ��������Ľ���ؿ���Ӧ����֪ͨ��Lua����
*
*/
#ifndef __SINGLE_STAGE_ROXY_H__
#define __SINGLE_STAGE_ROXY_H__

#include "EventProxy.h"
#include "BufferData.h"

class CSingleStageProxy : public CEventProxy
{
public:
	CSingleStageProxy();
	virtual ~CSingleStageProxy();

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