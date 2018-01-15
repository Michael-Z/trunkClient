#ifndef __CSB_EFFECT_H__
#define __CSB_EFFECT_H__

#include "cocostudio/ActionTimeline/CSLoader.h"
#include "cocostudio/ActionTimeline/CCActionTimeline.h"
#include "Effect.h"

class CCsbEffect : public CEffect
{
public:
    CCsbEffect();
    virtual ~CCsbEffect();

    virtual bool init(const std::string& path, bool isDoLayout = true);
    virtual bool init(int dir, const EffectConfItem* conf, const std::string& path);
    virtual void onEnter();
    virtual void onExit();
    // ����ָ������
    virtual bool playAnimate(const std::string& animate);
    // ����ָ�����������ڶ�����������Զ��Ƴ�
    virtual bool playAnimateAutoRemove(const std::string& animate);
    // ���ò����ٶ�
    virtual void setAnimateSpeed(float speed)
    {
        if (NULL != m_pAction)
        {
            m_pAction->setTimeSpeed(speed);
        }
    }
    // ��ȡ��Ч�ڵ�
    cocos2d::Node* getEffectNode() { return m_pNode; }

private:
    std::string m_Path;
    cocos2d::Node* m_pNode;
    cocostudio::timeline::ActionTimeline* m_pAction;
};

#endif