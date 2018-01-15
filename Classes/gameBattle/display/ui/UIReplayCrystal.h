/*
*   �ط�ˮ��UI
*   1. ��UI�Լ���Ӧ��ģ�ͣ�����ʼ�����ȼ���״̬�����ģ�
*
*/
#ifndef __UI_REPLAYCRYSTAL_H__
#define __UI_REPLAYCRYSTAL_H__

#include "cocos2d.h"
#include "BattleHelper.h"
#include "BattleModels.h"
#include "GameComm.h"

class CUIReplayCrystalComponent : public cocos2d::Component
{
public:
    CUIReplayCrystalComponent();
    ~CUIReplayCrystalComponent();

    bool init(cocos2d::Node* crystalNode, CHero* hero);
    void update(float dt);

    /*
     * @brief  ����ˮ���ȼ�
     */
    void setCurLevel();

    /*
     * @brief  ���õ�ǰˮ������
     */
    void setCurCrystal();

    /*
     * @brief  �������ˮ������
     */
    void setMaxCrystal();

    /*
     * @brief  ����ˮ���ȼ��ı�
     */
    void setCurLevelText();

    /*
     * @brief  ���õ�ǰˮ�������ı�
     */
    void setCurCrystalText();

    /*
     * @brief  �������ˮ�������ı�
     */
    void setMaxCrystalText();
    
    /*
    * @brief   ����ˮ�����ؽ�����
    */
    void setLoadingBarPercent();

    /**
    * @brief  ����ˮ����״̬��Ч
    */
    void playCrystalEffect(float dt);

    /**
    * @brief  ����ˮ�������������Ч
    */
    void playBuffEffect();

    void onCrystalUpgrade(void* data);

private:

    void playEffect(const char* effName);

    enum LayerZorder
    {
        LZ_CRYSTALSTATE = 11,			// ִ�����ֲ�
    };

    /*
    @brief  ��ȡ���صĽ���
    */
    float getLoadPercent();

private:
    int m_CurLevel;
    float m_DefaultSpeed;
    float m_CurCrystal;
    int m_MaxCrystal;
    bool m_bEffectPlaying;
    CHero* m_Hero;
    cocos2d::ui::Text* m_LevelText;             // ��ǰˮ���ȼ�
    cocos2d::ui::Text* m_CurCrystalText;        // ��ǰˮ������
    cocos2d::ui::Text* m_MaxCrystalText;        // ���ˮ������
    cocos2d::ui::Text* m_AddGemText;            // �Ӷ�ˮ������
    cocos2d::ui::LoadingBar* m_LoadingBar;      // ˮ�����ؽ���
    cocostudio::timeline::ActionTimeline* m_CrystalAnimation;       // ˮ�����嶯��
    cocostudio::timeline::ActionTimeline* m_AddGemAnimation;        // �Ӷ�ˮ������
    cocostudio::Armature* m_BuffAnimation;
};

#endif
