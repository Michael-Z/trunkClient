/*
*   �ٻ�ʦ���(pvp��ʾ)
*   1. ��UI�Լ���Ӧ��ģ��, ����ʼ���ٻ�ʦ��Ϣ(ͷ��, ����, Ѫ��)
*   2. ʵ��Ѫ��������Ч��, Ѫ���͵ľ���
*   3. ʵ�ֵ�����ֵķ���
*   4. ʵ�ֶԷ��������ֵ���ʾ
*   2016-5-31 by Сǿ
*/

#ifndef __UI_SUMMONER_H__
#define __UI_SUMMONER_H__

#include "cocos2d.h"
#include "BattleHelper.h"
#include "GameComm.h"

struct TalkInfo
{
    int userID;
    int talkID;
};

class CUISummonerComponent : public cocos2d::Component
{
public:
    CUISummonerComponent();
    virtual ~CUISummonerComponent();

    bool init(cocos2d::Node* SummonerPanelNode, CRole* role, bool isSelf);
    void onExit();
    void update(float dt);
    void updateState(float dt);
    void updateHp(float dt);

private:
    enum HPState
    {
        kNormal,    // ����
        kTips,      // ������ʾ
        kZero,      // Ѫ��Ϊ0
    };

    void initAnimation(cocos2d::Node* summonerPanelNode);
    void initHpBarInfo(cocos2d::Node* summonerPanelNode);

    void setBarPercent(cocos2d::ui::LoadingBar* bar, const float& finalPercent, float& curPercent, float& speed, const float dt);
    HPState CountHPState(const float& hpPercent);

    void onShowTalk(void* data);
    void onMaskClick(cocos2d::Ref* object);
    void onHeroClick(cocos2d::Ref* object);
    void onTalkClick(cocos2d::Ref* object);

    // ���������˵��
    void robotTalk();

private:
    bool            m_bIsSelf;
    HPState         m_HPState;
    HPState         m_preHPState;
    CRole*          m_pRole;

    cocos2d::ui::Layout*        m_pMaskLayer;   // ���β�

    cocos2d::ui::LoadingBar*    m_pHPBar;       // Ѫ����
    cocos2d::ui::LoadingBar*    m_pHPBarD;      // Ѫ����

    cocos2d::ui::ImageView* m_pHeadImg;  // ͷ��
    
    cocos2d::ui::Text*      m_pTalkText; // ˵���Ļ�

    int m_nMyUserID;
    int m_nNextTalkTime;        // �������´�˵��ʱ��
    int m_nLanIDs[6];           // ����ID

    float   m_fInterval;        // Ѫ���ı����ʱ��(0.2s)
    float   m_fHPMaxValue;      // ���Ѫ��ֵ
    float   m_fHPValue;         // ��ǰѪ��ֵ
    float   m_fHPPercent;       // ǰѪ����ʾ��ֵ
    float   m_fHPDPercent;      // �ײ�Ѫ����ʾ��ֵ
    float   m_fHPFinalPercent;  // ����Ѫ��������ֵ
    float   m_fHPSpeed;         // ǰѪ���ĸı��ٶ�
    float   m_fHPDSpeed;        // �ײ�Ѫ���ı���ٶ�

    cocostudio::timeline::ActionTimeline*   m_pHpBarAction;
    cocostudio::timeline::ActionTimeline*   m_pHeadAction;
    cocostudio::timeline::ActionTimeline*   m_pTalkAction;      // ˵���Ļ�
    cocostudio::timeline::ActionTimeline*   m_pTalkPanelAction; // �������ֵ����
};

#endif // !__UI_SUMMONER_H__