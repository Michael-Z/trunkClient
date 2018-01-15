/*
*   �ط�ս��������UI
*   1. ��ʼ��ս�����������ģ�飨���ܡ�ˮ������Ƭ��������ʾ����
*   2. ʵ��ս������ʱ����
*   3. ʵ��ս����ͣ���˳�UI
*   4. ʵ��TIPS��ʾ����
*
*   2017-2-21 by wsy copy ��ү
*/
#ifndef __UI_REPLAYBATTLE_MAIN_H__
#define __UI_REPLAYBATTLE_MAIN_H__

#include "cocos2d.h"
#include "BattleHelper.h"
#include "GameComm.h"
#include "UIActionTipsBar.h"

class CUIReplayBattleMain : public cocos2d::Layer
{
public:
    CUIReplayBattleMain();
    virtual ~CUIReplayBattleMain();

    virtual bool init();
    virtual void update(float delta);
    virtual void onEnter();
    virtual void onExit();

    void onShowTip(void* data);
    void onFightStart(void* data);

private:
    void showTips(const char* tips, float delay = 2.0f);
    void updateTime();
    void updateHurt();
    void updateDeath();
    void playDownTimeEffect(int time);
    // �л�������Χ
    void changeBackgroudMood();
    bool checkMoodCondition(const VecVecInt& conditionList);
    bool timeCondition(float time);
    bool soilderCondition(int count);
    bool crystalCondition(int level);

    void onBtnAdd(Ref* object);
    void onBtnMinus(Ref* object);
	void onBtnSpeed(Ref* object);

    // �����ٻ�ʦӢ�ۿ�ƬUI��Ϣ
    void setSummonerHeroInfo(bool isLeft);

    void test();
    void hero(Ref* ref);
    void skill(Ref* ref);
    void crystal(Ref* ref);

private:
    bool m_bUnDoDeathCamera;
    int m_nDownTime;
    int m_nHurt;
	float m_nSpeed;
    float m_fMoodParam;

    CUIActionTipsBar* m_ActionTipsBar;
    cocostudio::timeline::ActionTimeline* m_TimeAct;
    cocostudio::timeline::ActionTimeline* m_BattleAct;
    cocostudio::timeline::ActionTimeline* m_SpeedBtnAct;
	cocos2d::Node* m_BattleUI;
	cocos2d::Node* m_SkillTip;
	cocos2d::Node* m_Tips;
	cocos2d::Node* m_DownTimeNode;
	cocos2d::Node* m_HurtNode;
    cocos2d::ui::Text* m_TimeText;
    cocos2d::ui::TextAtlas* m_DownTimeText;
    cocos2d::ui::TextAtlas* m_HurtText;
    CBattleHelper* m_Helper;

	cocos2d::ui::Button* m_ScaleAddButton;
	cocos2d::ui::Button* m_ScaleMinusButton;
	cocos2d::ui::Button* m_SpeedButton;
};

#endif