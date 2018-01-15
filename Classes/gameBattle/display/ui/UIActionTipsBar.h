/*
*   ������ʾ��
*   1. ���Է����ִ����ĳЩ�������Զ���Ӷ�����ʾ��ť����ʾ��
*   2. ��ʾ���������3��������ʾ��ť�����ȵİ�ť�ᱻ�Ƴ�
*   3. ������ʾ��ťͣ��һ��ʱ�����Զ�moveby������
*   4. ���������ʾ��ť�ᵯ��Tip��ϸ��ʾ��ͬʱֻ�����һ����ʾ����ʾͣ��һ��ʱ��󵭳���
*   5. ��������κεط�����ʾ��ʧ
*
*   2015-12-26 by ��ү
*/
#ifndef __UI_ACTION_TIPS_BAR_H__
#define __UI_ACTION_TIPS_BAR_H__

#include <cocos2d.h>
#include <queue>
#include <vector>

class CUIActionTipsBar;

// ������ʾ��ť
class CUIActionIcon : public cocos2d::Node
{
public:
    CUIActionIcon();
    virtual ~CUIActionIcon();

    bool init(CUIActionTipsBar* parent, const char* icon, const char* name, const char* desc);
    // ����ӵ������лص�������ʼִ��Action
    void onEnter();
    // ���Ƴ�ʱ�ص���m_Parent->popActionIcon
    void onFinish();
    // ����ص���������m_Parent->showTip
    void onClick(cocos2d::Ref* object);

private:
    cocos2d::Node* m_CsbNode;
    CUIActionTipsBar* m_Parent;
    const char* m_Icon;
    const char* m_Name;
    const char* m_Desc;
};

template <typename T>
class CEventManager;

class CUIActionTipsBar : public cocos2d::Node
{
public:
    CUIActionTipsBar();
    virtual ~CUIActionTipsBar();

    bool init(bool isleft);
    void onExit();

    // ���˷���������ص�
    void onEnemyAction(void* data);

    // ��ʾ����ֱ������
    void showTip(const char* icon, const char* name, const char* desc, const cocos2d::Vec2& pos);
    void hideTip();
    void popActionIcon();

private:
    void addActionIcon(const char* icon, const char* name, const char* desc);

private:
    bool m_bLeftSide;
    cocos2d::Node* m_TipNode;
    std::list<CUIActionIcon*> m_Queue;
    std::vector<CUIActionIcon*> m_Caches;
	CEventManager<int>*	m_pEventManager;
};

#endif
