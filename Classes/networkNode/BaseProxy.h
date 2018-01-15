#ifndef __BASEPROXY_H__
#define __BASEPROXY_H__

#include "KxCSComm.h"

class CBaseProxy : public Ref
{
public:
    CBaseProxy(void);
    virtual ~CBaseProxy(void);

public:
    //ҵ���߼�������յ���Ϣ
    virtual int onRecv(char *buffer, int len) = 0;
    //�߼������귢����Ϣ
    virtual int send(char *buffer, int len) = 0;
    //�쳣, errorΪ��������, �������Ϊ��Ϣ
    virtual int onError(int error, int tag = 0 , char *data = 0) = 0;
    //����
    virtual void clear() { };
};

#endif 
