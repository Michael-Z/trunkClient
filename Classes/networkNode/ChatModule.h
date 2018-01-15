#ifndef __CHAT_MODULE_H__
#define __CHAT_MODULE_H__

#include "BaseModule.h"

class CChatModule : 
    public CBaseModule
{
public:
    CChatModule(void);
    ~CChatModule(void);

    // �����յ�������
    virtual void processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target);
    // ������ֵĴ���
    virtual void processError(KxServer::IKxComm *target);
    // �����¼�
    virtual void processEvent(int eventId, KxServer::IKxComm* target);
};

#endif 
