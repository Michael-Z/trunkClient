/*
 * ��ʾ���
 * 1.������ʾ����Ļ���
 *
 * 2014-12-19 by ��ү
 */
#ifndef __DISPLAY_COMPONENT_H__
#define __DISPLAY_COMPONENT_H__

#include "KxCSComm.h"

class CDisplayComponent : public Component
{
public:
    CDisplayComponent();
    virtual ~CDisplayComponent();

    // ����Ϊ����ʾ���󣬽���ʾ�ڵ��TAG����ΪMAIN_DISPLAY
    virtual void setMainDisplay(bool isMain)
    {
    }
};

#endif
