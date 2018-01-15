/*
 * ս������
 * 
 * 1.��ȡ�ⲿ�����ݣ��û���Ϣ��������Ϣ�ȵ�
 * 2.�ڿͻ��˼���ս��������ʾ���ݣ�����ʼ��
 * 3.����BattleHelper��EventManager��һϵ�ж��󣬲���ʼ��
 * 4.ս��ָ�������
 *
 * 2014-12-18 By ��ү
 */
#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "KxCSComm.h"
#include "BattleHelper.h"
#include "BattleModels.h"
#include "GameObject.h"

class CBattleHelper;
class CPVPSerialize;

class CBattleLayer : public Layer
{
public:
    CBattleLayer();
    virtual ~CBattleLayer();

public:
    // ��������
    static Scene* createNewScene(CRoomModel* room);
    // �����ط�/�ز�����
    static Scene* createReplayScene(CRoomModel* room);

    // ��ʼ��
    virtual bool init(CRoomModel* room);
    // ������ʼ
    virtual void onEnter();
    // �����˳�
    virtual void onExit();
    // �߼�����
    virtual void update(float delta);

    // ս���¼���Ӧ
    void onResponse(void* data);
	// �ؿ���ս�����¼��ص�
	void onStageResponse(void *data);

    // ����ÿ������߼�֡
    inline void setTickSpeed(int tick) { m_fTickDelta = 1.0 / tick; }
    // ��ǰÿ֡�ӳ�
    inline float getTickDelta() { return m_fTickDelta; }
    CRoomModel* getRoomModel() { return m_pRoomModel; }

    void pauseBattle(bool isPauseAction = true);
    void resumeBattle();
    void quitBattle();

    void doReady();
    bool replayAgain();
private:
    // ��Ϸ�߼�����
    bool logicUpdate(float delta);
    // ֡����Ϣ
    void openDebugInfo();
    void showDebugInfo();

private:
	bool			m_bIsPauseAction;     // �Ƿ���ͣ����
    bool            m_bIsPause;           // �Ƿ���ͣս��
    float           m_fDelta;             // �ϴ�ִ���߼�����������ȥ��ʱ��
    float           m_fTickDelta;         // ÿ�߼�֡�����ӳ�
    CBattleHelper*  m_pBattle;            // battlehelper
	CRoomModel*     m_pRoomModel;         // ����ģ��
	CPVPSerialize*  m_pPVPSerialize;	  // pvp�����
};

#endif
