/*
*
*   2015-11-19 by ��ү
*/
#ifndef __DEBUGER_MODULE_H__
#define __DEBUGER_MODULE_H__

#include "KXServer.h"
#include "KxDebuger.h"

namespace kxdebuger {

class DebugerModule :
    public KxServer::IKxModule
{
public:
    DebugerModule();
    virtual ~DebugerModule();

    virtual void processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target);
    virtual void processError(KxServer::IKxComm *target);

    // ������Ҫ��������ݳ���, tcp�ְ��ӿ�
    virtual int processLength(char* buffer, unsigned int len);

    // ע��һ��Service�����ڴ������
    virtual bool addService(int serviceId, IService* service);
    // ע��һ��Service
    virtual void removeService(int serviceId);
    // �������Service
    virtual void clearService();

private:
    std::map<int, IService*> m_Services;
};

}

#endif