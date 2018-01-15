#include "DebugerModule.h"


namespace kxdebuger {

DebugerModule::DebugerModule()
{
    
}


DebugerModule::~DebugerModule()
{
    clearService();
}

void DebugerModule::processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target)
{
    Head* head = reinterpret_cast<Head*>(buffer);
    auto service = m_Services.find(head->serviceId);
    if (service != m_Services.end())
    {
        service->second->process(head->actionId,
            buffer + sizeof(Head), len - sizeof(Head), target);
    }
}

void DebugerModule::processError(KxServer::IKxComm *target)
{

}

int DebugerModule::processLength(char* buffer, unsigned int len)
{
    if (len < sizeof(Head))
    {
        return sizeof(Head); 
    }

    // ����һ�����ĳ���, �ɰ�ͷ����, �����ĳ��Ȱ�����ͷ
    return reinterpret_cast<Head*>(buffer)->length;

}

bool DebugerModule::addService(int serviceId, IService* service)
{
    if (m_Services.find(serviceId) == m_Services.end())
    {
        m_Services[serviceId] = service;
        service->retain();
        return true;
    }
    return false;
}

// ע��һ��Service
void DebugerModule::removeService(int serviceId)
{
    auto service = m_Services.find(serviceId);
    if (service == m_Services.end())
    {
        service->second->release();
        m_Services.erase(service);
    }
}

// �������Service
void DebugerModule::clearService()
{
    for (auto& item : m_Services)
    {
        item.second->release();
    }
    m_Services.clear();
}

}