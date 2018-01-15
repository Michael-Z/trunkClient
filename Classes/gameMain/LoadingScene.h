//���س���

#ifndef _SUM_LOADING_SCENE
#define _SUM_LOADING_SCENE

#include "KxCSComm.h"
#include "BattleModels.h"

class CLoadingScene : public Scene
{
public:
	static Scene* create(CRoomModel* room);
	CREATE_FUNC(CLoadingScene);
	virtual bool init();
};

class CLoadingLayer : public Layer
{
public:
	CREATE_FUNC(CLoadingLayer);
	virtual bool init();
	virtual void onEnter();

	cocos2d::Label* lb;
	int m_nResCount;
	int m_nFinish;
	CRoomModel* RoomModel;

private:
	void ChangeFrameOriginalAndRect(const char* fileName);	//�ı�frame��һЩ����,ʹ��ʹ�ô��ͼƬ���зŴ󲻱�ɫ, ͼƬ��x�������3����
};

#endif