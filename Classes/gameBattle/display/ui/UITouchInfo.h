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
#ifndef __UI_TOUCH_INFO_H__
#define __UI_TOUCH_INFO_H__

#include "cocos2d.h"
#include "GameComm.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "SuperRichText.h"
#include "Skill.h"
#else
#include "commonFrame/mixedCode/SuperRichText.h"
#include "gameBattle/logic/Skill.h"
#endif

enum TOUCH_UI_TYPE
{
	TOUCH_UI_TYPE_HERO,
	TOUCH_UI_TYPE_SKILL
};

struct UITouchInfo_UI_SKILL
{
	cocos2d::Node* m_pRoot;
	cocos2d::ui::Text* m_tSkillName;		//��������
	cocos2d::ui::Text* m_tSkillLevel;		//���ܵȼ�
	cocos2d::ui::Text* m_tCoolingTime;		//��ȴ
	cocos2d::ui::Text* m_tConsumePoint;		//����
	cocos2d::ui::Text* m_tIntroText;		//��ϸ����
};

struct UITouchInfo_UIRoot
{
	cocos2d::Node*	m_pSkillRoot;
	cocos2d::ui::Layout* m_pName;
	cocos2d::ui::Text* m_tSkillName;
	cocos2d::ui::Text* m_tSkillLevel;
	cocos2d::ui::Text* m_tCoolingTime;
	cocos2d::ui::Text* m_tConsumePoint;
	cocos2d::ui::Text* m_tIntroText;

	cocos2d::Node*	m_pHeroCardRoot;
	cocos2d::ui::Layout* m_pHeroCardTipsPanel;
	cocos2d::ui::Text*	m_pHeroName;
	cocos2d::ui::Text*	m_pAttackNum;
	cocos2d::ui::Text*	m_pBloodNum;
	UITouchInfo_UI_SKILL	m_pSkill[3];
};

class UITouchInfo : public cocos2d::Node
{
public:
	UITouchInfo();
	virtual ~UITouchInfo();
	static UITouchInfo* create();
    bool init();
	bool initUI();

	void updateUI(TOUCH_UI_TYPE uiType, CSoldierModel* heroId);
	void updateUI(TOUCH_UI_TYPE uiType, CSkill* skill);

	CC_SYNTHESIZE(bool, m_bIsOpen, isOpen);
	Size calculateFontSize(const char *str);
private:
	UITouchInfo_UIRoot m_UI;
};

#endif // __UI_TOUCH_INFO_H__
