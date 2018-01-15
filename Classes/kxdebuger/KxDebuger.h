/*
*  KxDebuger �������浥��
*  �ṩ�˳�ʼ����ע����񡢹���ȫ�ָ��µȹ���
*
*  2015-11-19 by ��ү 
*/
#ifndef __KX_DEBUGER_H__
#define __KX_DEBUGER_H__

#include "cocos2d.h"
#include "KxServer.h"
#include "IService.h"
#include "DebugerModule.h"
#include "KxDebugerProtocol.h"

namespace kxdebuger {

class DebugerModule;
class KxPollNode;
typedef std::function<void(float)> scheduleCallback;

class KxDebuger : public cocos2d::Ref
{
private:
    KxDebuger();
    virtual ~KxDebuger();

public:
    static KxDebuger* getInstance();
    static void destroy();

    // ��ʼ��TCP�������Լ���Ӧ��Module
    // ע��Ĭ�ϵ�Service
    // ����PollNode������ӵ������֪ͨ�ڵ���
    // �����������Ķ˿�
    virtual bool init(int port = 6666);
    // ע��һ��Service�����ڴ������
    virtual bool addService(int serviceId, IService* service);
    // ע��һ��Service
    virtual void removeService(int serviceId);
    // �������Service
    virtual void clearService();

    inline KxPollNode* getPoller() { return m_PollNode; }

private:
    KxPollNode* m_PollNode;
    DebugerModule* m_DebugerModule;
    static KxDebuger* m_Instance;
};

}
#endif
