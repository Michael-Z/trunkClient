/*
*   ˮ��UI
*   1. ��UI�Լ���Ӧ��ģ�ͣ�����ʼ�����ȼ���״̬�����ģ�
*   2. ��̬��ˮ�����㡢������״̬���л�
*   3. �������Ч��
*
*   2015-12-25 by ��ү
*/
#ifndef __UI_CRYSTAL_H__
#define __UI_CRYSTAL_H__

#include "cocos2d.h"
#include "BattleHelper.h"
#include "BattleModels.h"
#include "GameComm.h"

class CUICrystalComponent : public cocos2d::Component
{
public:
    CUICrystalComponent();
    ~CUICrystalComponent();

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
     * @brief  ���õ�ǰ�����۸�
     */
    void setPrice();


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
     * @brief  ������������ˮ�������ı�
     */
    void setClimbTowerText();

    /*
     * @brief  ���õ�ǰ�����۸��ı�
     */
    void setPriceText();

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

private:

    enum LayerZorder
    {
        LZ_CRYSTALSTATE = 11,			// ִ�����ֲ�
    };

    void playEffect(const char* effName);
    void onClick(cocos2d::Ref* object);

    /*
    @brief  ��ȡ���صĽ���
    */
    float getLoadPercent();

private:
    int m_CurLevel;
    int m_Price;
    float m_CurCrystal;
    int m_MaxCrystal;
    float m_DefaultSpeed;
    bool m_bEffectPlaying;
    bool m_bLevelUpTips;
    bool m_bFingerTips;
    CHero* m_Hero;
    cocos2d::ui::Text* m_LevelText;             // ��ǰˮ���ȼ�
    cocos2d::ui::Text* m_CurCrystalText;        // ��ǰˮ������
    cocos2d::ui::Text* m_MaxCrystalText;        // ���ˮ������
    cocos2d::ui::Text* m_ClimbTowerText;        // ��������ˮ������
    cocos2d::ui::Text* m_AddGemText;            // �Ӷ�ˮ������
    cocos2d::ui::Text* m_PriceText;             // ��ǰ�����۸�
    cocos2d::ui::LoadingBar* m_LoadingBar;      // ˮ�����ؽ���
    cocostudio::timeline::ActionTimeline* m_CrystalAnimation;       // ˮ�����嶯��
    cocostudio::timeline::ActionTimeline* m_TextAnimation;          // �ײ����ֶ���
    cocostudio::timeline::ActionTimeline* m_AddGemAnimation;        // �Ӷ�ˮ������
    cocostudio::Armature* m_BuffAnimation;
    cocos2d::Node* m_TipsFinger;                // ����ˮ����ʾ��ָ
    cocostudio::timeline::ActionTimeline* m_TipsFingerAnimation;        // �Ӷ�ˮ������
};

#endif
