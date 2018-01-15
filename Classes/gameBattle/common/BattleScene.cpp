#include "BattleScene.h"
#include "cocostudio/CocoStudio.h"

#include "Game.h"
#include "ConfOther.h"
#include "Protocol.h"
#include "BattleProtocol.h"
#include "StageProtocol.h"
#include "PvpProtocol.h"
#include "NetBattleProxy.h"
#include "SingleBattleProxy.h"
#include "ProxyManager.h"
#include "GameNetworkNode.h"

#include "UIBattleMain.h"
#include "UIReplayBattleMain.h"

#include "BattleDragLayer.h"
#include "BattleHelper.h"
#include "Hero.h"
#include "Bullet.h"
#include "CallSoldier.h"
#include "UIEffect.h"
#include "Events.h"
#include "DisplayCommon.h"
#include "UIEffect.h"
#include "GameModel.h"
#include "PVPSerialize.h"

#include "LuaSummonerBase.h"
#include "RoleDisplayComponent.h"
#include "ResManager.h"
#include "ConfLanguage.h"
#include "FMODAudioEngine.h"

#include "ResPool.h"
#include "TipBloodNode.h"

using namespace std;
using namespace cocostudio;
USING_NS_CC;

float alldt = 0.0f;
struct timeval lastupdate;
bool bFightStart = false;

void initProxy(CBaseProxy* proxy)
{
    CProxyManager::getInstance()->addProxy(CMD_BATTLE, proxy);
}

Scene* CBattleLayer::createNewScene(CRoomModel* room)
{
    CHECK_RETURN_NULL(NULL != room);

    // ��ʼ���������
    CBaseProxy* proxy = NULL;
    switch (room->getBattleType())
    {
    case EBATTLE_PVP:
        proxy = new CNetBattleProxy();
        (dynamic_cast<CNetBattleProxy*>(proxy))->init(CGame::getInstance()->EventMgr);
        break;
	//case EBATTLE_CHAPTER:
    default:
        proxy = new CSingleBattleProxy();
        (dynamic_cast<CSingleBattleProxy*>(proxy))->init(CGame::getInstance()->EventMgr);
        break;
    }
    initProxy(proxy);

    // ��������ҪԤ����
    Scene *scene = Scene::create();
	// ���Ų�
	Layer *scaleLayer = Layer::create();
	
    CBattleLayer* battleLayer = new CBattleLayer();
    CBattleDragLayer* dragLayer = new CBattleDragLayer;
    CUIBattleMain* uiLayer = new CUIBattleMain();
    do
    {
        CHECK_BREAK(battleLayer->init(room));
		CHECK_BREAK(dragLayer->init(room->getStageId(), scaleLayer, battleLayer));
        CHECK_BREAK(uiLayer->init());

        battleLayer->setName("BattleLayer");
		scaleLayer->addChild(battleLayer, SLZ_BATTLE1, BLT_BATTLE);

        dragLayer->setName("BattleDragLayer");
        dragLayer->setTag(BLT_DRAG);
		scaleLayer->addChild(dragLayer);
		scaleLayer->setAnchorPoint(Vec2(0, 0));
		scaleLayer->setName("ScaleLayer");
		scene->addChild(scaleLayer, LZ_SCALE);

        uiLayer->setName("BattleUILayer");
        scene->addChild(uiLayer, LZ_UI, BLT_UI);

        CUIEffectManager::getInstance()->init(scene);
    } while (false);

    SAFE_RELEASE(battleLayer);
    SAFE_RELEASE(dragLayer);
    SAFE_RELEASE(uiLayer);

    return scene;
}

Scene* CBattleLayer::createReplayScene(CRoomModel* room)
{
    CHECK_RETURN_NULL(NULL != room);

    // ��ʼ���������
    CBaseProxy* proxy = new CSingleBattleProxy();
    (dynamic_cast<CSingleBattleProxy*>(proxy))->init(CGame::getInstance()->EventMgr);

    initProxy(proxy);

    Scene *scene = Scene::create();
    // ���Ų�
    Layer *scaleLayer = Layer::create();

    CBattleLayer* battleLayer = new CBattleLayer();
    CBattleDragLayer* dragLayer = new CBattleDragLayer;
    CUIReplayBattleMain* uiLayer = new CUIReplayBattleMain();
    do
    {
        CHECK_BREAK(battleLayer->init(room));
        CHECK_BREAK(dragLayer->init(room->getStageId(), scaleLayer, battleLayer));
        CHECK_BREAK(uiLayer->init());

        battleLayer->setName("BattleLayer");
        scaleLayer->addChild(battleLayer, SLZ_BATTLE1, BLT_BATTLE);

        dragLayer->setName("BattleDragLayer");
        dragLayer->setTag(BLT_DRAG);
        scaleLayer->addChild(dragLayer);
        scaleLayer->setAnchorPoint(Vec2(0, 0));
        scaleLayer->setName("ScaleLayer");
        scene->addChild(scaleLayer, LZ_SCALE);

        uiLayer->setName("BattleUILayer");
        scene->addChild(uiLayer, LZ_UI, BLT_UI);

        CUIEffectManager::getInstance()->init(scene);
    } while (false);

    SAFE_RELEASE(battleLayer);
    SAFE_RELEASE(dragLayer);
    SAFE_RELEASE(uiLayer);

    return scene;
}

CBattleLayer::CBattleLayer()
: m_bIsPauseAction(false)
, m_bIsPause(false)
, m_fDelta(0.0f)
, m_pBattle(NULL)
, m_pRoomModel(NULL)
, m_pPVPSerialize(NULL)
{
    setTickSpeed(10);
}

CBattleLayer::~CBattleLayer()
{
    initProxy(NULL);
    SAFE_DELETE(m_pBattle);
	SAFE_DELETE(m_pPVPSerialize);
    CGame::getInstance()->BattleHelper = NULL;
    //SAFE_DELETE(m_pRoom);
}

bool CBattleLayer::init(CRoomModel* roomModel)
{
    bFightStart = false;

    CHECK_RETURN(NULL != roomModel);
	CHECK_RETURN(Layer::init());
	// ����ģ��
	m_pRoomModel = roomModel;
    // ע����Ӧ�¼�
    for (int i = CMD_BAT_SCBEGIN + 1; i < CMD_BAT_SCEND; i++)
    {
        CGame::getInstance()->EventMgr->addEventHandle(MakeCommand(CMD_BATTLE, i),
            this, CALLBACK_FUNCV(CBattleLayer::onResponse));
    }
	// ������ʼ��Ϸ��Ӧ, ���ڲ���"Ԥ����ʼ"
	CGame::getInstance()->EventMgr->addEventHandle(MakeCommand(CMD_PVP, CMD_PVP_START_SC),
		this, CALLBACK_FUNCV(CBattleLayer::onResponse));
	CGame::getInstance()->EventMgr->addEventHandle(MakeCommand(CMD_PVP, CMD_PVP_OPPRECONNECT_SC),
		this, CALLBACK_FUNCV(CBattleLayer::onResponse));
	CGame::getInstance()->EventMgr->addEventHandle(MakeCommand(CMD_PVP, CMD_PVP_OPPDISCONNECT_SC),
		this, CALLBACK_FUNCV(CBattleLayer::onResponse));

    CBattlePlayerModel* myModel = NULL;
    CBattlePlayerModel* otherModel = NULL;
    map<int, CBattlePlayerModel*>& players = roomModel->getPlayers();
    for (auto model : players)
    {
        if (model.first == CGame::getInstance()->UserId
            || model.first == EDefaultPlayer)
        {
            myModel = model.second;
        }
        else
        {
            otherModel = model.second;
        }
    }

    // ���ű�������
    const StageConfItem* pStageConfItem = queryConfStage(roomModel->getStageId());
    CHECK_RETURN(NULL != pStageConfItem);
    const StageSceneConfItem *stageSceneConf = queryConfStageScene(pStageConfItem->StageSenceID);
    CHECK_RETURN(NULL != stageSceneConf);
    onLuaPlayBgMusic(stageSceneConf->BgMusicId);

	// ��ʼ��BattleHelper
	m_pBattle = new CBattleHelper();
	CGame::getInstance()->BattleHelper = m_pBattle;
	if (!m_pBattle->init(roomModel, myModel, otherModel, CGame::getInstance()->EventMgr, this))
	{
		return false;
	}

    alldt = 0.0f;

    if (m_pRoomModel->getBattleType() == EBATTLE_PVP)
    {
        m_pPVPSerialize = new CPVPSerialize(m_pBattle);

        // ����С����
        CGame::getInstance()->EventMgr->addEventHandle(MakeCommand(CMD_LOGIN, CMD_LOGIN_RECONECT_CS),
            this, CALLBACK_FUNCV(CBattleLayer::onResponse));
    }
    else if (m_pRoomModel->getBattleType() == EBATTLE_PVPREPLAY)
    {
        m_pPVPSerialize = new CPVPSerialize(m_pBattle);
    }

    // ִ�г����ٻ����߼�
    if (m_pBattle->getBattleType() != EBATTLE_PVP)
    {
        for (std::vector<SSceneCall>::const_iterator iter = pStageConfItem->SceneCall.begin(); 
            iter != pStageConfItem->SceneCall.end(); ++iter)
        {
            if (!m_pBattle->createCallSoldier(EDefaultScene, iter->callID, 1, iter->callPosX, iter->callPosY))
            {
                return false;
            }
        }
    }

    // ֡����ʾ
    //openDebugInfo();
    return true;
}

void CBattleLayer::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getRunningScene()->setonEnterTransitionDidFinishCallback([this](){
        if (m_pBattle->getBattleType() == EBATTLE_PVP)
        {
            onLuaBattleStart(0);
        }
        else
        {
            onLuaBattleStart(m_pRoomModel->getStageId());
        }
    });

    Director::getInstance()->getEventDispatcher()->setMultiTouchEnable(true);
//#ifdef COCOS2D_DEBUG
    // ��һ֡��ʱ��
    m_fDelta = 0.0f;
    gettimeofday(&lastupdate, nullptr);
//#endif

    // ��ͣ, ���Ƕ�������
    pauseBattle(false);

    // pvp
    if (m_pBattle->getBattleType() == EBATTLE_PVP)
    {
        CPvpModel *pPvpModel = CGameModel::getInstance()->getPvpModel();
        // �Ƿ�����
        if (pPvpModel->isReconnect())
        {
            // �������л�
            CGame::getInstance()->sendRequest(CMD_BATTLE, CMD_BAT_PVPUPDATECS, NULL, 0);
            // ��ִ�����µľ�ͷ
            return;
        }
    }
    else if (m_pBattle->getBattleType() == EBATTLE_PVPREPLAY)
    {
        // ���л�һ��,�Ա�ط�ʹ��
        m_pPVPSerialize->serializeCommandBuffData();
    }

    doReady();
}

void CBattleLayer::onExit()
{
    Layer::onExit();
    CResPool::destory();
    TipBloodNode::destory();
    CGame::getInstance()->EventMgr->removeEventHandle(this);
    CUIEffectManager::getInstance()->uninit();
    CUIEffectManager::destroy();
    Director::getInstance()->getEventDispatcher()->setMultiTouchEnable(false);
	//��������״̬
	CPvpModel *pPvpModel = CGameModel::getInstance()->getPvpModel();
	if (pPvpModel->isReconnect())
	{
		pPvpModel->setBattleId(0);
		pPvpModel->setReconnect(false);
	}
}

void CBattleLayer::onResponse(void* data)
{
    // ������Ϸ���������ս������
    Head* head = reinterpret_cast<Head*>(data);
    if (NULL == head)
    {
        return;
    }

	if (head->MainCommand() == CMD_PVP)
	{
		if (head->SubCommand() == CMD_PVP_START_SC)
		{
            m_fDelta = 0.0f;
			resumeBattle();
//#if COCOS2D_DEBUG
			gettimeofday(&lastupdate, nullptr);
//#endif
            auto *pUILayer = Director::getInstance()->getRunningScene()->getChildByName("BattleUILayer");
			if (NULL != pUILayer)
            {
				auto readyGo = pUILayer->getChildByName("ReadyGo");
                if (NULL != readyGo)
                {
                    readyGo->setVisible(true);
                    auto action = getCsbAnimation(readyGo);
                    action->setLastFrameCallFunc([readyGo, action](){
                        // ����ս����ʼ��ʾ
                        if (!bFightStart)
                        {
                            bFightStart = true;
                            CGame::getInstance()->EventMgr->raiseEvent(BattleEventFightStartTips, NULL);
                        }
                        readyGo->removeFromParent();
                        action->clearLastFrameCallFunc();
                    });
                    playCsbAnimation(readyGo, "Go");
                }
            }
		}
		else if (head->SubCommand() == CMD_PVP_OPPRECONNECT_SC)
		{
			//��ʾ���������
			CGame::getInstance()->EventMgr->raiseEvent(BattleEventShowTips,
				(void*)(getLanguageString(CONF_UI_LAN, 842)));
		}
		else if (head->SubCommand() == CMD_PVP_OPPDISCONNECT_SC)
		{
			//��ʾ����˳���Ϸ
			CGame::getInstance()->EventMgr->raiseEvent(BattleEventShowTips,
				(void*)(getLanguageString(CONF_UI_LAN, 840)));
		}
	}
	else if (head->MainCommand() == CMD_BATTLE)
	{
		switch (head->SubCommand())
		{
		case CMD_BAT_PVPCOMMANDSC:
			{
                // ���
				BattleCommandInfo* info = reinterpret_cast<BattleCommandInfo*>(head->data());
				if (NULL != m_pBattle)
				{
					// �ж��Ƿ���Ҫ�����л�
					if (m_pRoomModel->getBattleType() == EBATTLE_PVP
                        && m_pPVPSerialize->processSerializeByCommand(*info))
					{
                        //m_fDelta += (m_pBattle->CurTick - m_pBattle->GameTick) * m_fTickDelta;
                        // ���¼�����Ҫ׷�ص�ʱ�䣬׷�ص�alldt
                        m_fDelta = alldt - m_fDelta - m_pBattle->GameTick * m_fTickDelta;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
                        if (CGame::getInstance()->isDebug())
                        {
                            // �����ӳ���ʾ
                            Label* lb = Label::create();
                            lb->setSystemFontSize(28);
                            auto winsize = Director::getInstance()->getWinSize();
                            string msg = " Current Tick " + toolToStr(m_pBattle->GameTick)
                                + " Command Tick " + toolToStr(info->Tick);
                            lb->setPosition(winsize * 0.5f);
                            lb->setString(msg);
                            getParent()->addChild(lb);
                            lb->runAction(Sequence::create(FadeIn::create(1.5f),
                                FadeOut::create(1.5f), RemoveSelf::create(), NULL));
                        }
#endif
					}

					// ��������Ҫ���⴦�������������Ƿ�ִ���˷����л������ﶼ��Ҫ���룩
                    m_pBattle->insertBattleCommand(*info);
				}
			}
			break;
			// �������·���������
		case CMD_BAT_PVPUPDATESC:
			{
				CBufferData buffdata;
				// ע��ȥ��ͷ��Head�Ĵ�С
				buffdata.init((char*)head->data(), head->length - sizeof(Head));
				m_pBattle->unserialize(buffdata);
				
                // ��Ϸdt
                float gameDt = m_pBattle->GameTick / 10.0f;
                // ��������С��������ִ������ˢ�£���������ʱ��alldt������ʧЧ
                // ���alldtС��gameDt����˵���Ǵ�����
                // ��ΪgameDt�Ƿ������ĵ�ǰʵ��ʱ�䣨���Ͱ�ʱ����alldt�ǿͻ��˵ĵ�ǰʵ��ʱ��
                // ���ͻ����ܵñȷ������ʱ����˵���Ǵ�����������������ʱ alldt��0��ʼ������������ʱ����һֱ���ܣ�
                if (alldt < gameDt)
                {
                    alldt = gameDt;
                }
                m_pPVPSerialize->SaveReconnectData(&buffdata);
				resumeBattle();
			}
			break;
			// PVPս��������Ӧ
		case CMD_BAT_PVPENDSC:
			{
				// ֹͣս��
				// m_pBattle->onBattleOver();
				// ִ��BattleHelper.onBattleOver
				unscheduleUpdate();
				onLuaBattleOver();
			}
			break;
		}
	}
	else if (head->MainCommand() == CMD_LOGIN)
	{
		switch (head->SubCommand())
		{
			case CMD_LOGIN_RECONECT_CS:
			{
                KXLOGDEBUG("SEND PVP UpdateCS");
                // �����δִ�е�����ָ��
                /*const vector<BattleCommandInfo>& cmdQueue = m_pBattle->getBattleCommandQueue();
                int curCmdIndex = m_pBattle->getCurExecutCommandCount();
                BattlePvpUpdateCS updateCS;
                if (cmdQueue.size() > 0)
                {
                    int lastUserCmd = 0;
                    for (int i = cmdQueue.size() - 1; i >= curCmdIndex; --i)
                    {
                        if (cmdQueue[i].CommandId != CommandCallSolider)
                        {
                            if (lastUserCmd == 0)
                            {
                                lastUserCmd = i;
                            }
                            if (cmdQueue[i].Tick > m_pBattle->GameTick)
                            {
                                // ����Ҫ�����л�
                                return;
                            }
                            else
                            {
                                // ���뷴���л�
                                break;
                            }
                        }
                    }
                    updateCS.lastCmdGameTick = cmdQueue[lastUserCmd].Tick;
                    updateCS.lastCmdIndex = lastUserCmd;
                }
                else
                {
                    // ����ˢ��
                    updateCS.lastCmdGameTick = 0;
                    updateCS.lastCmdIndex = 0;
                }
                // �������л�
                CGame::getInstance()->sendRequest(CMD_BATTLE, CMD_BAT_PVPUPDATECS, &updateCS, sizeof(updateCS));*/
                Head head;
                head.length = sizeof(head);
                head.MakeCommand(CMD_BATTLE, CMD_BAT_PVPUPDATECS);
                CGameNetworkNode::getInstance()->sendData(reinterpret_cast<char*>(&head), sizeof(head));
			}
			break;
		}
	}
}

void CBattleLayer::update(float delta)
{
    if (m_pRoomModel->getBattleType() == EBATTLE_PVP)
	{
        // ս��δ��ʼ��ս��δ������������PVP�Զ����л�
        if (!m_pBattle->IsBattleOver)
        {
            m_pPVPSerialize->update(delta);
        }

//#if COCOS2D_DEBUG
        // DEBUGģʽ�²���Ҫ��ʱ�����У������ΪDirector::calculateDeltaTime��
        // ��DEBUGģʽ�»Ὣ����0.2f��delta��Ϊ1 / 60.0
        struct timeval now;
		gettimeofday(&now, nullptr);
        // PVP��Ҫ����׷��
        delta = (now.tv_sec - lastupdate.tv_sec) + (now.tv_usec - lastupdate.tv_usec) / 1000000.0f;
        lastupdate = now;
//#endif
    }

    alldt += delta;
    m_pBattle->CurTick = static_cast<int>(alldt * 10);
    // ÿһ֡���ۼ�delta
    m_fDelta += delta;

    if (alldt >= 0.1f)
    {
        // ����ս����ʼ��ʾ
        if (!bFightStart)
        {
            bFightStart = true;
            CGame::getInstance()->EventMgr->raiseEvent(BattleEventFightStartTips, NULL);
        }
    }

    // ������ʱdelta������ʱ�߼�֡��Ƶ��Ҳ����ű��
    // ���߼�ִ֡�е��߼�ʱ�䲻��
    int execCount = 0;
    while (m_fDelta >= m_fTickDelta)
    {
        m_fDelta -= m_fTickDelta;
        if (!logicUpdate(m_fTickDelta))
        {
            // ��Ϸ������Ҫǿ���˳���ѭ��
            break;
        }
        if (++execCount >= 3)
        {
            break;
        }
    }

    // PVP����֡����ʾ
    // showDebugInfo();
}

bool CBattleLayer::logicUpdate(float delta)
{
    if (m_pBattle->IsBattleOver)
    {
        return false;
    }

    // ���ս������ ���� �����Ϸ�����Ὣm_pBattle->IsBattleOver����Ϊtrue
    if (m_pBattle->checkBattleOver())
    {
        // �Ƚ�ս������д�����ģ��
        m_pBattle->onBattleOver();
        CFMODAudioEngine::getInstance()->clearAllEffects();
        switch (m_pBattle->getBattleType())
        {
            // PVP�ɺ��֪ͨս������
        case EBATTLE_PVP:
            break;
        default:
            unscheduleUpdate();
            onLuaBattleOver();
            return false;
        }
    }
    else
    {
        // ����ս���߼�
        m_pBattle->processBattle(delta);
	}
    return true;
}

void CBattleLayer::openDebugInfo()
{
    auto lb = Label::create();
    lb->setString("0");
    lb->setSystemFontSize(32);
    lb->setTag(7788);
    lb->setPosition(Vec2(480, 600));
    addChild(lb);

    lb = Label::create();
    lb->setString("0");
    lb->setSystemFontSize(32);
    lb->setTag(7799);
    lb->setPosition(Vec2(480, 560));
    addChild(lb);

    lb = Label::create();
    lb->setString("0");
    lb->setSystemFontSize(32);
    lb->setTag(7777);
    lb->setPosition(Vec2(480, 540));
    addChild(lb);
}

void CBattleLayer::showDebugInfo()
{
    Label* lb = (Label*)getChildByTag(7788);
    if (lb)
    {
        lb->setString(toolToStr(m_pBattle->GameTick));
    }

    lb = (Label*)getChildByTag(7799);
    if (lb)
    {
        lb->setString(toolToStr(alldt));
    }

    lb = (Label*)getChildByTag(7777);
    if (lb)
    {
        lb->setString(toolToStr(m_pBattle->CurTick));
    }
}

void pauseRole(CRole* role)
{
    role->pause();
    CAnimateComponent* displayCom = dynamic_cast<CAnimateComponent*>(
        role->getComponent("MainAnimate"));
    if (displayCom)
    {
        displayCom->pause();
    }
}

void pauseBullet(CBullet* bullet)
{
    bullet->pause();
    for (auto& child : bullet->getChildren())
    {
        child->pause();
    }
}

void resumeRole(CRole* role)
{
    role->resume();
    CAnimateComponent* displayCom = dynamic_cast<CAnimateComponent*>(
        role->getComponent("MainAnimate"));
    if (displayCom)
    {
        displayCom->resume();
    }
}

void resumeBullet(CBullet* bullet)
{
    bullet->resume();
    for (auto& child : bullet->getChildren())
    {
        child->resume();
    }
}

void CBattleLayer::pauseBattle(bool isPauseAction)
{
    if (!m_bIsPause)
    {
        m_bIsPause = true;
        unscheduleUpdate();
        if (m_bIsPauseAction != isPauseAction && isPauseAction)
        {
            m_bIsPauseAction = isPauseAction;
			pauseRole(m_pBattle->getMainRole(CampType::ECamp_Blue));
			pauseRole(m_pBattle->getMainRole(CampType::ECamp_Red));
			for (auto& role : m_pBattle->getRoleWithCamp(CampType::ECamp_Blue))
			{
				pauseRole(role);
			}
			for (auto& role : m_pBattle->getRoleWithCamp(CampType::ECamp_Red))
			{
				pauseRole(role);
			}
			for (auto& role : m_pBattle->getRoleWithCamp(CampType::ECamp_Neutral))
			{
				pauseRole(role);
			}
			for (auto& bullet : m_pBattle->getBullets())
			{
				pauseBullet(bullet);
			}
		}
    }
}

void CBattleLayer::resumeBattle()
{
    if (m_bIsPause)
    {
        m_bIsPause = false;
        scheduleUpdate();
		if (m_bIsPauseAction)
		{
            m_bIsPauseAction = false;
			resumeRole(m_pBattle->getMainRole(CampType::ECamp_Blue));
			resumeRole(m_pBattle->getMainRole(CampType::ECamp_Red));
			for (auto& role : m_pBattle->getRoleWithCamp(CampType::ECamp_Blue))
			{
				resumeRole(role);
			}
			for (auto& role : m_pBattle->getRoleWithCamp(CampType::ECamp_Red))
			{
				resumeRole(role);
			}
			for (auto& role : m_pBattle->getRoleWithCamp(CampType::ECamp_Neutral))
			{
				resumeRole(role);
			}
			for (auto& bullet : m_pBattle->getBullets())
			{
				resumeBullet(bullet);
			}
		}
    }
}

void CBattleLayer::quitBattle()
{
    m_pRoomModel->getSettleAccountModel()->setChallengeResult(CHALLENGE_CANCEL);
    m_pBattle->onBattleOver();
    unscheduleUpdate();
    onLuaQuitBattle();
}

void CBattleLayer::doReady()
{
    CBattleDragLayer *pDragLayer = dynamic_cast<CBattleDragLayer *>(
        Director::getInstance()->getRunningScene()->getChildByName("ScaleLayer")->getChildByTag(BLT_DRAG));

    //����Ԥ������
    auto *pUILayer = Director::getInstance()->getRunningScene()->getChildByName("BattleUILayer");
    auto readyGo = CResManager::getInstance()->getCsbNode("ui_new/f_fight/ReadyGo.csb");
    readyGo->setContentSize(Director::getInstance()->getWinSize());
    ui::Helper::doLayout(readyGo);
    if (NULL == readyGo->getParent())
    {
        auto action = CSLoader::createTimeline("ui_new/f_fight/ReadyGo.csb");
        readyGo->runAction(action);
        readyGo->setVisible(false);
        pUILayer->addChild(readyGo);
    }

    CHECK_RETURN_VOID(NULL != pDragLayer);
    //ִ������ͷ
    pDragLayer->doCamera(1);
    //���ûص�
    pDragLayer->setCameraFinishCallback([this, readyGo](){
        if (m_pBattle->getBattleType() != EBATTLE_PVP)
        {
            // ׼����Ч
            playSoundEffect(54);
            readyGo->setVisible(true);
            auto readyAction = getCsbAnimation(readyGo);
            if (NULL != readyAction)
            {
                readyAction->setLastFrameCallFunc([readyGo, readyAction](){
                    Head head = { sizeof(Head), MakeCommand(CMD_PVP, CMD_PVP_START_SC), 0 };
                    CGame::getInstance()->EventMgr->raiseEvent(MakeCommand(CMD_PVP, CMD_PVP_START_SC), (void*)&head);
                });
            }
            else
            {
                Head head = { sizeof(Head), MakeCommand(CMD_PVP, CMD_PVP_START_SC), 0 };
                CGame::getInstance()->EventMgr->raiseEvent(MakeCommand(CMD_PVP, CMD_PVP_START_SC), (void*)&head);
            }
            playCsbAnimation(readyGo, "Ready");
        }
    });
}

bool CBattleLayer::replayAgain()
{
    bFightStart = false;
    m_bIsPauseAction = false;
    m_bIsPause = true;
    alldt = 0.0f;
    m_fDelta = 0.0f;
    gettimeofday(&lastupdate, nullptr);
    m_pBattle->getSettleAccountModel()->resetSettle();

    CBattleDragLayer *pDragLayer = dynamic_cast<CBattleDragLayer *>(
        Director::getInstance()->getRunningScene()->getChildByName("ScaleLayer")->getChildByTag(BLT_DRAG));
    if (NULL != pDragLayer)
    {
        pDragLayer->resetLayer();
    }

    CHECK_RETURN(NULL != m_pPVPSerialize);
    m_pPVPSerialize->unSerializeLastPVPData();

    //ִ������ͷ+׼��
    doReady();

    return true;
}