#include "BaseModule.h"
#include "Protocol.h"
#include "cocos2d.h"

CBaseModule::CBaseModule(void)
{
}

CBaseModule::~CBaseModule(void)
{
}

int CBaseModule::processLength(char* buffer, unsigned int len)
{
    // ����һ�����ĳ���, �ɰ�ͷ����, �����ĳ��Ȱ�����ͷ
    if (len < sizeof(int))
    {
        return sizeof(int);
    }
    else
    {
        int ret = reinterpret_cast<Head*>(buffer)->length;
        CCLOG("processLength %d in len %d", ret, len);
        return ret;
    }
}
