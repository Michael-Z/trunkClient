/*
*   Ӣ�ۼ���UI
*   1. ��UI�Լ���Ӧ��ģ�ͣ�����ʼ����ͼ�ꡢ״̬�����ģ�
*   2. ʵ��CD����̬��ˮ�����㡢�����ͷŵ�״̬���л�
*
*   2015-12-25 by ��ү
*/
#ifndef __UI_REPLAYSKILL_H__
#define __UI_REPLAYSKILL_H__

#include "cocos2d.h"
#include "BattleHelper.h"
#include "BattleModels.h"
#include "GameComm.h"
#include "Hero.h"
#include "Skill.h"

class CUIReplaySkillComponent : public cocos2d::Component
{
public:
    CUIReplaySkillComponent();
    virtual ~CUIReplaySkillComponent();

    virtual void onExit();
    bool init(cocos2d::Node* skillNode, CHero* hero, int index);
    void update(float dt);
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
    SkillState checkSkillState();

private:
    int  m_Index;
    bool m_bEffectPlaying;

    CHero* m_Hero;
    CSkill* m_Skill;
    SkillState m_CurState;

    cocos2d::ui::Button*    m_SkillIcon;
    cocos2d::ProgressTimer* m_SkillMask;
    cocostudio::timeline::ActionTimeline* m_SkillAnimation;
};

#endif
