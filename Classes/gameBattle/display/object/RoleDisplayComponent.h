/*
* ��ɫ��ʾ���
* ����Owner��λ����ƽ����ʾλ��
* ����״̬������ָ������
*
* 2014-12-23 by ��ү
*/
#ifndef __ROLE_DISPLAY_COMPONENT_H__
#define __ROLE_DISPLAY_COMPONENT_H__

#include "DisplayComponent.h"
#include "ArmatureComponent.h"
#include "RoleComponent.h"
#include "Role.h"
#include "HPBar.h"
#include "Count.h"
#include "Effect.h"

#define DEATH_ACT_TAG 100001

class CRoleDisplayComponent : public CDisplayComponent
{
public:
    CRoleDisplayComponent();
    virtual ~CRoleDisplayComponent();

    virtual bool init(CRole* role, CAnimateComponent* animate, CRoleComponent* state);

    virtual void update(float delta);

    // �л���������
    void changeAnimate();
    // ��ȡ������������
    Node* getMainAnimate();
    // ˢ��Ѫ�����������˺��ı���ִ�����������߼�
    void playCountEffect(eHurtType hurtType, int hurtValue);
	CHPBar* getHPBar(){ return m_pHpBar; }
    // ˢ��Ѫ��
    void updateHpBar();
    // ��ʼ��debug�������
    void initDebug();

private:
    // ��ʼ��Ѫ��
    void initHpBar();
    // ������Ч
    // ���ش��������淽�����
    bool createEffects(Node* root, const VecInt& effIds, int zorder = 0, float delay = 0.0f, cocos2d::Vec2 pos = Vec2::ZERO);
    // �Ƴ���һ�ε���Ч
    void removePreEffects();
    // �Ƴ���һ�ε���Ч��ѭ����
    void stopPreMusics();
    // ��ȡ��ǰ�����ٶ�
    float getCurPlaySpeed();

private:
    Vec2 m_Offset;
    Vec2 m_TargetPos;
    Vec2 m_HpBarOffset;
    int m_nState;
    int m_nAnimateId;
    float m_fDuration;
    float m_fDelta;
    float m_PlaySpeed;
	CHPBar* m_pHpBar;
    CRoleComponent* m_pRoleCom;
	CAnimateComponent* m_pArmatureCom;
    CRole* m_pRole;
	std::vector<CEffect*> m_vEffectNode;
    std::vector<int> m_vMusics;
};

#endif
