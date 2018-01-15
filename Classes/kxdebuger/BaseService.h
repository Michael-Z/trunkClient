/*
*   ��������
*   1.�ṩ��Ϸ��ͣ�ͻָ�����
*   2.�ṩ��Ϸ����ǰ������
*
*   2015-11-19 By ��ү
*/

#ifndef __BASE_SERVICE_H__
#define __BASE_SERVICE_H__

#include "KxDebuger.h"

namespace kxdebuger {

class BaseService : public IService
{
public:
    BaseService();
    virtual ~BaseService();

    virtual void process(int actionId, void* data, int len, KxServer::IKxComm *target);

private:
    void actionStepSelector(float dt);
};

}

#endif
