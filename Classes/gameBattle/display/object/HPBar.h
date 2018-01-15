/************************************************
* HPBar_ ����Ѫ��
*
* ���������ע������:
	pow: ��ֵΪ100
*
* ���ߣ�
* ���ڣ�2015��12��25��
************************************************/

#ifndef __HPBAR_H__
#define __HPBAR_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "RoleComm.h"
using cocos2d::ui::Text;
using cocos2d::ui::LoadingBar;

//Ѫ������
enum eBarSizeType
{
	kHP_s = 0,			        // СѪ��
	kHP_m,				        // ��Ѫ��
	kHP_l,				        // ��Ѫ��
	kHPSummoner,		        // �ٻ�ʦ

	KHPMagic_s = 10,	        // СѪ��+ħ��
	KHPMagic_m,			        // ��Ѫ��+ħ��
	KHPMagic_l,			        // ��Ѫ��+ħ��
	kHPMagicSummoner,	        // �ٻ�ʦ+ħ��

	kHPRage_s = 20,		        // СѪ��+ŭ��
	kHPRage_m,			        // ��Ѫ��+ŭ��
	kHPRage_l,			        // ��Ѫ��+ŭ��
	kHPRageSummoner,	        // �ٻ�ʦ+ŭ��

    kHPMagicRage_s = 30,        // СѪ��ŭ��
    kHPMagicRage_m = 31,        // ��Ѫ��ŭ��
    kHPMagicRage_l = 32,        // ��Ѫ��ŭ��
    kHPMagicRageSummoner = 33,  // �ٻ�ʦѪ��ŭ��
};

class CHPBar : public cocos2d::Node
{
public:
	CHPBar(eBarSizeType sizeType, CampType camp, float maxHP, float shield = 0, float magic = 0, float rage = 0);
    ~CHPBar();
    static CHPBar* create(eBarSizeType sizeType, CampType camp, float maxHP, float shield, float magic = 0, float rage = 0);
	bool init();

    virtual void onExit();

	void setHP(float hp);
	//void setMaxHP(float maxHP);
	void setShield(float shield);
    void setMagic(float magic);
    void setRage(float rage);
	void setName(std::string name);
	eBarSizeType getSizeType(){ return m_eSizeType; }
	CampType getCamp(){ return m_eCamp; }
	void ChangeCamp(CampType camp);
	void setReduceTime(float dt);	// ��ʾѪ���仯��ʱ��
	
private:
	// �������ʹ�����csbѪ��
	void createCsb();
	// ��ʼ��Ѫ������ʾ
	void initShow();

	// ���ý������İٷֱ�
	void setBarPercent(LoadingBar* bar, const float& percent, float& curPercent);
	void setHPPercent(float percent);
	void setHPDPercent(float percent);
	void setShieldPercent(float percent);
    void setMagicPercent(float percent);
    void setMagicDPercent(float percent);
    void setRagePercent(float percent);
    void setRageDPercent(float percent);
	
	// ������ֵ������
	void boundValue(float& value, float minValue = 0, float maxValue = 0);
	// ���ݵ�ǰ��ֵ�����Ѫ�ͻ������յĳ���
	void countHPShieldFinal();
	// �����ٶ����ý������İٷֱ�
    inline void setPercentBySpeed(LoadingBar* bar, float& targetPercent, float& curPercent, float& speed, float dt)
    {
        if (speed == 0)
        {
            return;
        }
        curPercent += speed * dt;
        if ((speed > 0 && curPercent >= targetPercent)
            || (speed < 0 && curPercent <= targetPercent))
        {
            curPercent = targetPercent;
            speed = 0;
        }
        setBarPercent(bar, curPercent, curPercent);
    }

	void update(float dt);
private:
	eBarSizeType		m_eSizeType;
	CampType			m_eCamp;

	cocos2d::Node*			m_pCsbHP;			// Ѫ��csb
    cocos2d::Node*          m_pCsbMagic;        // ������ʾħ����������
    cocos2d::Node*          m_pCsbRage;         // ������ʾŭ����������
	Text*					m_pTextName;		// �ٻ�ʦ����
	LoadingBar*				m_pBarHP;			// Ѫ��
	LoadingBar*				m_pBarHPD;			// Ѫ����
	LoadingBar*				m_pBarShield;		// ����
    LoadingBar*             m_pBarMagic;        // ħ��
    LoadingBar*             m_pBarMagicD;       // ħ����
    LoadingBar*             m_pBarRage;         // ŭ��
    LoadingBar*             m_pBarRageD;        // ŭ����
	cocostudio::timeline::ActionTimeline* m_pCsbHPAct;	//�л���Ӫ

	// Ѫ����ǰֵ
	float	m_fHP;
	float	m_fMaxHP;
	float	m_fShield;
	//float	m_fPow;
    float   m_fMagic;
    float   m_fRage;

	// ������ʾֵ
	float	m_fFinalHPPercent;
	float	m_fFinalShieldPercent;
	//float	m_fFinalPowPercent;
    float   m_fFinalMagicPercent;
    float   m_fFinalRagePercent;

	// ��ǰ��ʾ��ֵ
	float	m_fCurHPPercent;
	float	m_fCurHPDPercent;
	float	m_fCurShieldPercent;
    float   m_fCurMagicPercent;
    float   m_fCurMagicDPercent;
    float   m_fCurRagePercent;
    float   m_fCurRageDPercent;

	// �仯�ٶ�
	float	m_fHPSpeed;
	float	m_fHPDSpeed;
	float	m_fShieldSpeed;
    float   m_fMagicSpeed;
    float   m_fMagicDSpeed;
    float   m_fRageSpeed;
    float   m_fRageDSpeed;

	// ���ӻ���� ��Ҫ��ʱ��
	float	m_fReduceTime;

    std::string m_HPPath;   // hp csb·��
};

#endif