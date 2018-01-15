#ifndef __ARMATURE_EFFECT_H__
#define __ARMATURE_EFFECT_H__

#include "cocostudio/CocoStudio.h"
#include "Effect.h"

class CArmatureEffect : public CEffect
{
public:
	CArmatureEffect();
	~CArmatureEffect();

    virtual bool init(int dir, const EffectConfItem* conf);
    virtual bool init(const std::string& armature);
	virtual void onEnter();
    // ����ָ������
    virtual bool playAnimate(const std::string& animate);
    // ����ָ�����������ڶ�����������Զ��Ƴ�
    virtual bool playAnimateAutoRemove(const std::string& animate);    
    // ���ò����ٶ�
    virtual void setAnimateSpeed(float speed)
    {
        if (NULL != m_pAnimation)
        {
            m_pAnimation->getAnimation()->setSpeedScale(speed);
        }
    }
    // ��ȡ��Ч�ڵ�
    cocos2d::Node* getEffectNode() { return m_pAnimation; }

private:
    cocostudio::Armature* m_pAnimation;
};

#endif