/*
* �ٻ�ʦ���˵�����
*
* ����������Ϸ��ȫ�ֱ���
* ������Ϸ�е�����ģ��
*
* 2015-1-16 by ��ү
*/
#ifndef __GAME_H__
#define __GAME_H__

#include "KxCSComm.h"
#include "BattleModels.h"
#include "EventManager.h"
#include "BattleHelper.h"

enum EPfType
{
    EDebug,     // ����
    EQQHall,    // QQ����
    EAnySDK,    // anysdk
};

class CGame
{
private:
    CGame();
    virtual ~CGame();

public:
	static CGame* getInstance();

	static void destory();

	bool init();
    // ע��, �������¼�executeʱ, ���øýӿ�(raiseEvent)����, �������ƻ���һ�ε��õ��ڴ�����, �����±���
    void sendRequest(int maincmd, int subcmd, void *data, int len);
    // �Ƿ��ǵ���ģʽ
    bool isDebug(){ return m_bDebug; }
    // ƽ̨����
    EPfType getPfType(){ return m_nPfType; }

public:
	CPlayerModel*       User;
    CEventManager<int>* EventMgr;
    CBattleHelper*      BattleHelper;    // ��ʱ����
    int                 UserId;

private:
	static CGame*       m_Instance;

    bool m_bDebug;      // ���Կ���
    EPfType m_nPfType;  // ƽ̨����
};

#endif // Game_h__
