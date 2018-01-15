/*
*   ��Ч����
*   ������Ч��ͨ�ýӿ�
*
*   2015-12-17 By ��ү
*/
#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "cocos2d.h"
#include "ConfFight.h"

typedef std::function<void()> EffectFinishCallback;

class CEffect : public cocos2d::Node
{
public:
    CEffect();
    virtual ~CEffect();

    virtual bool init();
    virtual bool init(int dir, const EffectConfItem* conf);
    virtual void onEnter();
    virtual void onExit();

    // ����ָ������
    virtual bool playAnimate(const std::string& animate) { return false; };
    // ����ָ�����������ڶ�����������Զ��Ƴ�
    virtual bool playAnimateAutoRemove(const std::string& animate) { return false; };
    // ���ò����ٶ�
    virtual void setAnimateSpeed(float speed) { }

    inline const EffectConfItem* getConf()
    {
        return m_pConf;
    }

    // ��ȡ��Ч�ڵ�
    virtual cocos2d::Node* getEffectNode() { return NULL; }

protected:
    int m_nDirection;
    const EffectConfItem* m_pConf;
};

#endif