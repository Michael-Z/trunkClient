#ifndef _SUM_UI_EFFECT_
#define _SUM_UI_EFFECT_

#include "KxCSComm.h"
#include "ConfFight.h"

class CUIEffectManager
{
public:
	static CUIEffectManager* getInstance();
    static void destroy();
    CUIEffectManager();

	void init(cocos2d::Node* battleScene);
    void uninit();
	//idΪUI��ЧID��posΪĿ�����λ�ã�λ��Ϊ��������λ��
	void execute(int id, const Vec2& pos = Vec2::ZERO);
    //ֹͣ��Ч
    void stopUIEffect();

private:
    void executeDark(UIEffectConfItem* conf);
    void executeShake(int shakeLevel, float shakeDelayTime, float shakeTime);
    void shakeNode(int tag, int shakeLevel, float shakeDelayTime, float shakeTime);
    cocos2d::Node* executeEffect(int zorder, int resId, const std::string& effAni);
    void executeDoodad(UIEffectConfItem* conf, const Vec2& pos);
    void findDoodads(cocos2d::Node* node);

private:
    Node* m_DarkNode;                       // �ڰ���
    Node* m_BackGround;                     // ������
    Node* m_Blink;                          // ������
    Node* m_Scene;                          // ս������
    std::list<Node*> m_Doodads;             // װ�����б�
    static CUIEffectManager* m_Instance;    // ��������
};

#endif	