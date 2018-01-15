#include "KxDebuger.h"
#include "KxPollNode.h"
#include "ListenerModule.h"
#include "DebugerModule.h"
#include "BaseService.h"
#include "NodeService.h"

using namespace cocos2d;
using namespace KxServer;

namespace kxdebuger {

KxDebuger* KxDebuger::m_Instance = nullptr;

KxDebuger::KxDebuger()
: m_PollNode(nullptr)
, m_DebugerModule(nullptr)
{
}

KxDebuger::~KxDebuger()
{
    if (m_PollNode)
    {
        if (Director::getInstance()->getNotificationNode() == m_PollNode)
        {
            Director::getInstance()->setNotificationNode(nullptr);
        }
        else
        {
            m_PollNode->removeFromParent();
        }
        CC_SAFE_RELEASE_NULL(m_PollNode);
    }

    KXSAFE_RELEASE(m_DebugerModule);
}

KxDebuger* KxDebuger::getInstance()
{
    if (m_Instance == nullptr)
    {
        m_Instance = new KxDebuger();
    }
    return m_Instance;
}

void KxDebuger::destroy()
{
    CC_SAFE_RELEASE_NULL(m_Instance);
}

bool KxDebuger::init(int port)
{
    if (m_PollNode)
    {
        return false;
    }

    m_PollNode = new KxPollNode();
    m_PollNode->init();

    if (Director::getInstance()->getNotificationNode())
    {
        Director::getInstance()->getNotificationNode()->addChild(m_PollNode);
    }
    else
    {
        Director::getInstance()->setNotificationNode(m_PollNode);
    }

    // ��������
    KxTCPListener* listener = new KxTCPListener();
    if (!listener->init() || !listener->listen(port))
    {
        CCLOG("KxTCPListener init faile, listen %d", port);
        listener->release();
        return false;
    }

    // ����ģ�飬���������������û���ӵ���ѯ�ڵ���
    ListenerModule* listenerModule = new ListenerModule();
    listener->setModule(listenerModule);
    listenerModule->release();

    // ����ģ�飬ÿ�������������û������Ե�����Ϸ
    m_DebugerModule = new DebugerModule();
    listener->setClientModule(m_DebugerModule);

    // Ϊ����ģ��ע��Ĭ�ϵķ���
    BaseService* baseService = new BaseService();
    addService(ServicesId::ServiceBase, baseService);
    baseService->release();
    NodeService* nodeService = new NodeService();
    addService(ServicesId::ServiceNode, nodeService);
    nodeService->release();

    // ��ӵ���ѯ
    m_PollNode->addCommObject(listener, listener->getPollType());
    listener->release();
    return true;
}

bool KxDebuger::addService(int serviceId, IService* service)
{
    if (m_DebugerModule)
    {
        return m_DebugerModule->addService(serviceId, service);
    }
    return false;
}

// ע��һ��Service
void KxDebuger::removeService(int serviceId)
{
    if (m_DebugerModule)
    {
        m_DebugerModule->removeService(serviceId);
    }
}

// �������Service
void KxDebuger::clearService()
{
    if (m_DebugerModule)
    {
        m_DebugerModule->clearService();
    }
}

}