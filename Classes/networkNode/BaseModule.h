/*
*  �̳���IBaseModule ��Ϊ������Ϸģ����ص�ģ�游��, ��Ҫͳһ���зְ�ʹ�ð�ͷ����
*
*/

#ifndef __CBASEMODULE_H__
#define __CBASEMODULE_H__

#include "KxServer.h"

class CBaseModule :
    public KxServer::IKxModule
{
public:
    CBaseModule(void);
    virtual ~CBaseModule(void);

    // ������Ҫ��������ݳ���, tcp�ְ��ӿ�
    virtual int processLength(char* buffer, unsigned int len);
};

#endif
