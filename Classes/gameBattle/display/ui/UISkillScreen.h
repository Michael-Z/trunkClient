/*******************************************************************
** ������:	¬��
** ��  ��:  2016-05-20 11:03
** ��  ��:	1.0
** ��  ��:  ��ʾѡ���ͷż��ܵ�����
** Ӧ  ��:
********************************************************************/
#ifndef __UI_SKILL_SCREEN_H__
#define __UI_SKILL_SCREEN_H__

#include "cocos2d.h"

class CUISkillScreen : public cocos2d::Layer
{
public:
    CUISkillScreen();
    virtual ~CUISkillScreen();

    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    CREATE_FUNC(CUISkillScreen);

private:
    cocos2d::Node* m_SkillScreen;
};

#endif // !__UI_SKILL_SCREEN_H__