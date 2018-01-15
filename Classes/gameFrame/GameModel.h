/*
* GameModelΪ��Ϸ��ģ�͵���
*
* 2015-8-4 by ��ү
*/
#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "BattleModels.h"
#include "ModelData.h"

class CGameModel
{
private:
    CGameModel();
    virtual ~CGameModel();

public:
    static CGameModel* getInstance();
    static void destroy();

    // ����ʱ���ö�γ�ʼ�������ݲ�ͬ��Э���ʼ����Ӧ��ģ��
    bool init(void* data);

    // �򿪷��䡢��ȡ���䡢�رշ���
    CRoomModel* openRoom();
    CRoomModel* openReplayRoom();
    CRoomModel* getRoom() { return m_Room; }
    void closeRoom() { CC_SAFE_DELETE(m_Room); }

    inline CUserModel* getUserModel() { return m_UserModel; }
    inline CBagModel* getBagModel() { return m_BagModel; }
    inline CEquipModel* getEquipModel() { return m_pEquipModel; }
    inline CHeroCardBagModel* getHeroCardBagModel() { return m_HeroCardBagModel; }
    inline CSummonersModel* getSummonersModel() { return m_SummonersModel; }
    inline CStageModel* getStageModel() { return m_StageModel; }
    inline CTeamModel* getTeamModel() { return m_pTeamModel; }
	inline CTaskModel* getTaskModel() { return m_pTaskModel; }
	inline CAchieveModel* getAchieveModel() { return m_pAchieveModel; }
	inline CGuideModel* getGuideModel() { return m_pGuideModel; }
	inline CUnionModel* getUnionModel(){ return m_pUnionModel; }
	inline CActivityInstanceModel* getActivityInstanceModel(){ return m_pActivityInstanceModel; }
	inline CMailModel* getMailModel(){ return m_pMailModel; }
	inline CGoldTestModel* getGoldTestModel(){ return m_pGoldTestModel; }
	inline CHeroTestModel* getHeroTestModel(){ return m_pHeroTestModel; }
	inline CTowerTestModel* getTowerTestModel(){ return m_pTowerTestModel; }
	inline CPersonalTaskModel* getPersonalTaskModel(){ return m_pPersonalTaskModel; }
	inline CTeamTaskModel* getTeamTaskModel(){ return m_pTeamTaskModel; }
    inline CPvpModel* getPvpModel(){ return m_pPvpModel; }
    inline CShopModel* getShopModel(){ return m_pShopModel; }
    inline COperateActiveModel* getOperateActiveModel(){ return m_pOperateActive; }
    inline CHeadModel* getHeadModel(){ return m_pHeadModel; }

    // ��ȡ����˵�ǰ��ϵͳʱ�䣨�������ʱ����м��㣩
    int getNow();
	// �Ƿ�Ϊ��ѳ鿨ʱ��
	bool isFreePickCard();
    // ��ȡ��¼ʱ��ʱ��
    inline int getLoginServerTime() { return m_LoginServerTime; }
    inline int getLoginClientTime() { return m_LoginClientTime; }

private:
    int m_LoginServerTime;      // ��¼ʱ����˵�ʱ������ɷ�����·���
    int m_LoginClientTime;      // ��¼ʱ�ͻ��˱��ص�ʱ���

    CRoomModel* m_Room;
    CUserModel* m_UserModel;
    CBagModel* m_BagModel;
    CEquipModel *m_pEquipModel;
    CHeroCardBagModel* m_HeroCardBagModel;
    CSummonersModel* m_SummonersModel;
    CStageModel* m_StageModel;
    CTeamModel *m_pTeamModel;
	CTaskModel* m_pTaskModel;
	CAchieveModel* m_pAchieveModel;
	CGuideModel* m_pGuideModel;
	CUnionModel* m_pUnionModel;
	CActivityInstanceModel* m_pActivityInstanceModel;
	CMailModel*	m_pMailModel;
	CGoldTestModel* m_pGoldTestModel;
	CHeroTestModel* m_pHeroTestModel;
	CTowerTestModel* m_pTowerTestModel;
	CPersonalTaskModel* m_pPersonalTaskModel;
	CTeamTaskModel*	m_pTeamTaskModel;
    CPvpModel* m_pPvpModel;
    CShopModel* m_pShopModel;
    COperateActiveModel* m_pOperateActive;
    CHeadModel* m_pHeadModel;
    static CGameModel* m_Instance;
};

#endif
