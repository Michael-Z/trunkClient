/*
*   Ӣ�ۿ�ƬUI
*   1. ��UI�Լ���Ӧ��ģ�ͣ�����ʼ������ͷ���츳�����ģ�
*   2. ʵ��CD����̬��ˮ�������״̬���л�
*   3. ʵ�����ͽ�����������߼�
*   4. ʵ��Ψһ����UI���ָ���
*   5. ʵ���������Ч������
*
*   ����߼����ȼ�������� > ��̬��� > ˮ�������� > Ψһ��� > CD���
*
*   2015-12-23 by ��ү
*/
#ifndef __UI_HERO_CARD_H__
#define __UI_HERO_CARD_H__

#include "cocos2d.h"
#include "BattleHelper.h"
#include "BattleModels.h"
#include "GameComm.h"
#include "UITouchInfo.h"
#include "UIBattleMain.h"

class CUIHeroCardComponent : public cocos2d::Component
{
public:
    CUIHeroCardComponent();
    virtual ~CUIHeroCardComponent();

    bool init(cocos2d::Node* cardNode, CHero* hero, int index);
    void update(float dt);

private:
    enum HeroCardState {
        HeroCardInvalid,
        HeroCardNormal,
        HeroCardCD,
        HeroCardLack,
    };

    // �㼶Zorder
    enum LayerZorder
    {
        LZ_SINGLE   = 13,			// Ψһ�ڵ��
        LZ_LOCK     = 14,			// ���ڵ��
        LZ_BUFF     = 15,           // buff�ڵ��
    };

    void playEffect(const char* effName);
    void onClick(cocos2d::Ref* object);
	void touchCallBack(Ref* ref, cocos2d::ui::Widget::TouchEventType type);

    HeroCardState checkModelState();

private:
    int m_Index;
	int m_iTouchingTime;
    float m_DefaultCD;
    float m_DefautlCost;
    bool m_bLock;
    bool m_bEffectPlaying;
	bool m_bTouching;

    CHero* m_Hero;
    CSoldierModel* m_Model;
    HeroCardState m_CurState;

	cocos2d::ui::Layout*    m_HeroItem;
	cocos2d::ui::Button* m_bButton;
    cocos2d::ui::Button*    m_HeroBg;               // Ӣ�۱���
    cocos2d::ui::Button*    m_HeroIcon;             // Ӣ��ͷ��
    cocos2d::ui::Text*      m_HeroCost;             // Ӣ��ˮ��
	cocos2d::ui::Text*      m_HeroOnly;
    cocos2d::Sprite*        m_LoadingMask;          // ���ֲ�

    cocostudio::timeline::ActionTimeline* m_CardAnimation;      // �������嶯��
    cocostudio::timeline::ActionTimeline* m_LightAnimation;     // �⶯��
    cocostudio::timeline::ActionTimeline* m_LockAnimation;      // ������
    cocostudio::timeline::ActionTimeline* m_BuffAnimation;      // Buff����(CD���ӡ�����)
};

#endif
