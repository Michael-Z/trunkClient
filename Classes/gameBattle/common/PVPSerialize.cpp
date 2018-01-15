#include "cocos2d.h"
#include "PVPSerialize.h"
#include "BattleHelper.h"
USING_NS_CC;

CPVPSerialize::CPVPSerialize(CBattleHelper* battleHelper)
	: m_nRecordCountDown(0)
	, m_nRecordInterval(3)
	, m_nRecordCount(3)
	, m_pCommandBuffData(NULL)
	, m_pBattleHelper(battleHelper)
	//, m_nTest(1)
{
	
}

CPVPSerialize::~CPVPSerialize()
{
	std::map<int, CBufferData*>::iterator iter = m_mapBufferData.begin();
	for (; iter != m_mapBufferData.end(); ++iter)
	{
		CC_SAFE_DELETE(iter->second);
	}
	m_mapBufferData.clear();

    std::list<CBufferData*>::iterator iterDiscard = m_DiscardBuffDataList.begin();
    for (; iterDiscard != m_DiscardBuffDataList.end(); ++iterDiscard)
    {
        CC_SAFE_DELETE(*iterDiscard);
    }
    m_DiscardBuffDataList.clear();

	CC_SAFE_DELETE(m_pCommandBuffData);
}

bool CPVPSerialize::processSerializeByCommand(const BattleCommandInfo& cmd)
{
	// pvp�����л�����
    //if ((random(1, 5) == 3 || cmd.Tick <= m_pBattleHelper->GameTick) && cmd.CommandId != BattleCommandType::CommandTalk)

    // ��ʱ��, ��Ҫ�����л� (˵���Ĳ�����ִ��ս���߼�, ���Ժ���)
    if (cmd.Tick <= m_pBattleHelper->GameTick && cmd.CommandId != BattleCommandType::CommandTalk)
	{
        // �������Ч�����л����ݣ���
        std::map<int, CBufferData*>::iterator iter = m_mapBufferData.begin();
        for (; iter != m_mapBufferData.end();)
        {
            if (iter->first >= cmd.Tick)
            {
                m_DiscardBuffDataList.push_back(iter->second);
                m_mapBufferData.erase(iter++);
            }
            else
            {
                ++iter;
            }
        }

        // �����л�
        std::map<int, CBufferData*>::reverse_iterator riter = m_mapBufferData.rbegin();
        if (riter != m_mapBufferData.rend())
        {
            KXLOGBATTLE("======star unserial to gameTick %d========", riter->first);
            unSerializePVPData(riter->second);
        }
        // û�ҵ�����λ�õķ����л�����, ʹ���ϴ�����ķ����л�����
        else if (m_pCommandBuffData != NULL)
		{
            KXLOGBATTLE("======star unserial to preCommand ========");
            // �̶����л�����Ϊ��, �Ż���������л�, ���Բ�������̶�����ķ����л�����, ֱ�ӷ����л��������л�����
			unSerializePVPData(m_pCommandBuffData);
		}
        return true;
	}
	else
	{
		// û����ʱ, ���л�������, ���������¼�����л�����
		serializeCommandBuffData();
		return false;
	}
}

void CPVPSerialize::serializeFixData()
{
    KXLOGBATTLE("======star serial fix data  gameTick %d========", m_pBattleHelper->GameTick);

	// ������������,��ɾ��������,����������
	if (m_mapBufferData.size() > 0 && m_mapBufferData.size() >= m_nRecordCount)
	{
        m_DiscardBuffDataList.push_back(m_mapBufferData.begin()->second);
		m_mapBufferData.erase(m_mapBufferData.begin());
	}

    // ȡ��һ��buffData
    CBufferData* data = NULL;
    if (m_DiscardBuffDataList.empty())
    {
        data = new CBufferData();
        data->init(65535);
        m_pBattleHelper->serialize(*data);
    }
    else
    {
        std::list<CBufferData*>::iterator iter = m_DiscardBuffDataList.begin();
        data = *iter;
        data->resetDataLength();
        data->setIsReadModel(false);
        m_DiscardBuffDataList.erase(iter);
        m_pBattleHelper->serialize(*data);
    }

	// �����´ι̶����л�ʱ��
	m_nRecordCountDown = m_nRecordInterval;

    // ͬһ֡���Ƿ������л�����, ����н������(���ܲ������)
    std::map<int, CBufferData*>::iterator iter = m_mapBufferData.find(m_pBattleHelper->GameTick);
    if (iter != m_mapBufferData.end())
    {
        m_DiscardBuffDataList.push_back(iter->second);
    }
	m_mapBufferData[m_pBattleHelper->GameTick] = data;
}

void CPVPSerialize::serializeCommandBuffData()
{
    KXLOGBATTLE("======star serial Command data  gameTick %d, curTick %d========",
        m_pBattleHelper->GameTick, m_pBattleHelper->CurTick);
    // lazy init
    if (NULL == m_pCommandBuffData)
    {
        CBufferData* data = new CBufferData();
        data->init(65535);
        m_pCommandBuffData = data;
    }
    else
    {
        m_pCommandBuffData->resetDataLength();
        m_pCommandBuffData->setIsReadModel(false);
    }
    
    m_pBattleHelper->serialize(*m_pCommandBuffData);

	// �����´ι̶����л�ʱ��
	m_nRecordCountDown = m_nRecordInterval;

	// ��������������л�����
	for (std::map<int, CBufferData*>::iterator iter = m_mapBufferData.begin();
		iter != m_mapBufferData.end(); ++iter)
	{
        m_DiscardBuffDataList.push_back(iter->second);
	}
	m_mapBufferData.clear();
}

void CPVPSerialize::unSerializePVPData(CBufferData* data)
{
    // �����´ι̶����л�ʱ��
    m_nRecordCountDown = m_nRecordInterval;
    data->resetOffset();
    data->setIsReadModel(true);
    m_pBattleHelper->unserialize(*data);
	KXLOGBATTLE("======unSerialize over =========");
}

void CPVPSerialize::SaveReconnectData(CBufferData* buffdata)
{
    // �����ʱ���л�����
    std::map<int, CBufferData*>::iterator iter = m_mapBufferData.begin();
    for (; iter != m_mapBufferData.end(); ++iter)
    {
        m_DiscardBuffDataList.push_back(iter->second);
    }
    m_mapBufferData.clear();

    // �����������л�����
    CC_SAFE_DELETE(m_pCommandBuffData);

    CBufferData* data = new CBufferData();
    if (data->init(buffdata))
    {
        m_pCommandBuffData = data;
    }
    else
    {
        KXLOGBATTLE("CPVPSerialize SaveReconnectData init buffData error!!!!");
        delete data;
    }
}

void CPVPSerialize::unSerializeLastPVPData()
{
    unSerializePVPData(m_pCommandBuffData);
}

void CPVPSerialize::update(float dt)
{
	m_nRecordCountDown -= dt;
	if (m_nRecordCountDown <= 0)
	{
		//���������ʱ��, ��ʼ���л�����
		serializeFixData();
	}
}

