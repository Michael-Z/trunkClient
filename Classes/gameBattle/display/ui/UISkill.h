/*
*   Ӣ�ۼ���UI
*   1. ��UI�Լ���Ӧ��ģ�ͣ�����ʼ����ͼ�ꡢ״̬�����ģ�
*   2. ʵ��CD����̬��ˮ�����㡢�����ͷŵ�״̬���л�
*
*   2015-12-25 by ��ү
*/
#ifndef __UI_SKILL_H__
#define __UI_SKILL_H__

#include "cocos2d.h"
#include "BattleHelper.h"
#include "BattleModels.h"
#include "GameComm.h"
#include "Hero.h"
#include "Skill.h"

class CUISkillComponent : public cocos2d::Component
{
public:
    CUISkillComponent();
    virtual ~CUISkillComponent();

    virtual void onExit();
    bool init(cocos2d::Node* skillNode, CHero* hero, int index);
    void update(float dt);
	void touchCallBack(Ref* psender, cocos2d::ui::Widget::TouchEventType type);
private:
    enum SkillState {
        SkillInvalid,
        SkillLock,
        SkillCD,
        SkillNormal,
        SkillLack,
        SkillExecuting,
    };

    enum LayerZorder
    {
        LZ_SKILLMASK   = 7,			// ִ�����ֲ�
    };

    void playEffect(const char* effName, bool loop = false);
    void onClick(cocos2d::Ref* object);
    SkillState checkSkillState();

    // ȡ���ͷż���
    void onSkillCancel(void* data);
    // �ɹ��ͷż���
    void onSkillPlay(void* data);

private:
    int  m_Index;
	int  m_iTouchingTime;
    bool m_bEffectPlaying;
    bool m_bWaitForClick;
	bool m_bTouching;

    CHero* m_Hero;
    CSkill* m_Skill;
    SkillState m_CurState;

    cocos2d::ui::Button*    m_SkillIcon;
	cocos2d::ui::Button*	m_bButton;
    cocos2d::ProgressTimer* m_SkillMask;
    cocostudio::timeline::ActionTimeline* m_SkillAnimation;
};

#endif
