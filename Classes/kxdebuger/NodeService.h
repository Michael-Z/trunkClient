/*
*   �ڵ����
*   1.�ṩ�˽ڵ�����ѯ����
*   2.�ṩ�˽ڵ���Ϣ��ѯ���޸Ĺ���
*   3.�ṩ�˽ڵ㼤�ɾ������
*
*   2015-11-19 by ��ү
*/
#ifndef __NODE_SERVICE_H__
#define __NODE_SERVICE_H__

#include "KxDebuger.h"

namespace kxdebuger {

class NodeService : public IService
{
public:
    NodeService();
    virtual ~NodeService();

    virtual void process(int actionId, void* data, int len, KxServer::IKxComm *target);
};

}

#endif
