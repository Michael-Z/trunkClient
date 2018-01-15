/*
����pvp��Ϸ�ӳٺ�, �����л��Ĵ���
*/
#ifndef __PVPSERIALIZE_H__
#define __PVPSERIALIZE_H__

#include <map>
#include "CommStructs.h"
#include "BufferData.h"

class CBattleHelper;

class CPVPSerialize
{
public:
	CPVPSerialize(CBattleHelper* battleHelper);
	~CPVPSerialize();

	// ���ܵ�����, �ж��Ƿ���Ҫ���л��ͷ����л�
	bool processSerializeByCommand(const BattleCommandInfo& cmd);
	// �̶�������л�����
	void serializeFixData();
	// �������л�����
	void serializeCommandBuffData();
	// �����л�����
	void unSerializePVPData(CBufferData* data);
    // ������, ��������ʱ�̵����л�����, ���ʱ�����л�����
    void SaveReconnectData(CBufferData* data);
    // �����л����µ�����
    void unSerializeLastPVPData();

	// ���ݸ��´���
	void update(float dt);

private:
	float m_nRecordCountDown;		// ��ʼ���л����ݵ���ʱ
	float m_nRecordInterval;		// ���л����ݵ�����¼�����ʱ��
	int m_nRecordCount;				// ���ݷ����л�����������
    //int m_nTest;

	CBattleHelper* m_pBattleHelper;

	// ���µĲ�����������л�����
	CBufferData* m_pCommandBuffData;

	// �̶���������л�����<GameTick, ���л�����>
	std::map<int, CBufferData*> m_mapBufferData;
    std::list<CBufferData*> m_DiscardBuffDataList;  // ������buffdata
};

#endif