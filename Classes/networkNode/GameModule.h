#ifndef __GAMEMODULE_H__
#define __GAMEMODULE_H__

#include "BaseModule.h"

class CGameModule : 
    public CBaseModule
{
public:
    CGameModule(void);
    ~CGameModule(void);

    // �����յ�������
    virtual void processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target);
    // ������ֵĴ���
    virtual void processError(KxServer::IKxComm *target);
    // �����¼�
    virtual void processEvent(int eventId, KxServer::IKxComm* target);
};

#endif 
