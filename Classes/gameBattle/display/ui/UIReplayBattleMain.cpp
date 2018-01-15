#include "UIReplayBattleMain.h"
#include "UIReplaySkill.h"
#include "UIReplayCrystal.h"
#include "UISummoner.h"
#include "UIReplayHeroCard.h"
#include "UIActionTipsBar.h"
#include "ResManager.h"
#include "DisplayCommon.h"
#include "GameComm.h"
#include "Game.h"
#include "Hero.h"
#include "Events.h"
#include "ConfLanguage.h"
#include "UIPause.h"
#include "BattleDragLayer.h"
#include "LuaSummonerBase.h"
#include "UIEffect.h"
#include "FMODAudioEngine.h"
#include "ResPool.h"


#include "Protocol.h"
#include "BattleProtocol.h"

USING_NS_CC;
using namespace std;
using namespace ui;

CUIReplayBattleMain::CUIReplayBattleMain()
    :m_bUnDoDeathCamera(true)
    , m_nDownTime(0)
    , m_fMoodParam(0.0f)
	, m_nSpeed(1)
    , m_BattleAct(nullptr)
    , m_SpeedBtnAct(nullptr)
{
}


CUIReplayBattleMain::~CUIReplayBattleMain()
{
    Director::getInstance()->getScheduler()->setTimeScale(1.f);
}

bool CUIReplayBattleMain::init()
{
    bool ret = Layer::init();
	m_BattleUI = CResManager::getInstance()->getCsbNode("ui_new/f_fight/Fight_Replay.csb");
    m_BattleUI->setContentSize(Director::getInstance()->getWinSize());
    addChild(m_BattleUI);

    m_BattleAct = CSLoader::createTimeline("ui_new/f_fight/Fight_Replay.csb");
    m_BattleUI->runAction(m_BattleAct);
    m_BattleAct->play("Hide", false);

    m_Helper = CGame::getInstance()->BattleHelper;
    m_nHurt = 0;
    m_bUnDoDeathCamera = true;

    auto pauseBtn = findChild<Button>(m_BattleUI, "MainPanel/PlayAndPause_Button");
    pauseBtn->addClickEventListener([](Ref* sender){
        // ��ͣ�ص�
        CUIPause* pause = CUIPause::create();
        pause->setLocalZOrder(LZ_UI + 10);
        Director::getInstance()->getRunningScene()->addChild(pause);
    });

    // ��ȡ���ؽڵ�
    m_SkillTip = findChild<Node>(m_BattleUI, "MainPanel/Tips_1");
    m_Tips = findChild<Node>(m_BattleUI, "MainPanel/Tips_2");
    m_TimeText = findChild<Text>(m_BattleUI, "MainPanel/TimeItem/TimePanel/TimeTex");
    m_TimeAct = getCsbAnimation(findChild<Node>(m_BattleUI, "MainPanel/TimeItem"));
	// ��ʾ���ֿ�ʼʱ����, ��Ҫ����ʱ����ʾ
	m_Tips->setVisible(false);

    // ����ʱ�ڵ㡪��Ĭ������
    m_DownTimeNode = findChild<Node>(m_BattleUI, "MainPanel/CountdownTime");
    m_DownTimeText = findChild<TextAtlas>(m_DownTimeNode, "TimeAtlasLabel");
    m_DownTimeNode->setVisible(false);

    // ����˫�������Ϣ
    setSummonerHeroInfo(true);
    setSummonerHeroInfo(false);

	//�Ŵ� ��С
	m_ScaleAddButton = findChild<Button>(m_BattleUI, "MainPanel/ScaleAddButton");
	m_ScaleMinusButton = findChild<Button>(m_BattleUI, "MainPanel/ScaleMinusButton");
#ifdef WIN32
	m_ScaleAddButton->addClickEventListener(CC_CALLBACK_1(CUIReplayBattleMain::onBtnAdd, this));
	m_ScaleMinusButton->addClickEventListener(CC_CALLBACK_1(CUIReplayBattleMain::onBtnMinus, this));
#else
	m_ScaleAddButton->setVisible(false);
	m_ScaleMinusButton->setVisible(false);
#endif

    //���ټ���
	m_SpeedButton = findChild<Button>(m_BattleUI, "MainPanel/SpeedButton");
    m_SpeedBtnAct = CSLoader::createTimeline("ui_new/f_fight/effect/SpeedButton.csb");
    m_SpeedButton->runAction(m_SpeedBtnAct);
    m_SpeedButton->addClickEventListener(CC_CALLBACK_1(CUIReplayBattleMain::onBtnSpeed, this));
    m_nSpeed = 1.0f;

    // ע������¼�����
    m_Helper->pEventManager->addEventHandle(BattleEventShowTips,
        this, CALLBACK_FUNCV(CUIReplayBattleMain::onShowTip));
    m_Helper->pEventManager->addEventHandle(BattleEventFightStartTips,
        this, CALLBACK_FUNCV(CUIReplayBattleMain::onFightStart));

    // CUIActionTipsBar
    m_ActionTipsBar = new CUIActionTipsBar();
    m_ActionTipsBar->init(false);
    addChild(m_ActionTipsBar);
    m_ActionTipsBar->release();

    test();
    scheduleUpdate();
    return ret;
}

void CUIReplayBattleMain::update(float delta)
{
    updateTime();
    updateHurt();
    updateDeath();
}

void CUIReplayBattleMain::onEnter()
{
    Layer::onEnter();
    ui::Helper::doLayout(this);
}

void CUIReplayBattleMain::onExit()
{
    Layer::onExit();
    m_Helper->pEventManager->removeEventHandle(this);
}

void CUIReplayBattleMain::onShowTip(void* data)
{
    const char* tips = reinterpret_cast<const char*>(data);
    showTips(tips);
}

void CUIReplayBattleMain::onFightStart(void* data)
{
    if (!m_BattleUI->isVisible())
    {
        m_BattleUI->setVisible(true);
    }

    m_bUnDoDeathCamera = true;
    m_fMoodParam = 0.0f;
    m_nSpeed = 1;
    m_BattleAct->play("Appear", false);
    m_SpeedBtnAct->play("X1", false);
    playSoundEffect(49);
}

void CUIReplayBattleMain::showTips(const char* tips, float delay /* = 2.0f */)
{
	m_Tips->setVisible(true);
    // ǿ�Ƹ���Tips
	findChild<Text>(m_Tips, "TipsLabel")->setString(tips);
    m_Tips->stopAllActions();
    m_Tips->setOpacity(255);
    m_Tips->runAction(Sequence::create(
		FadeIn::create(0.2f),
        DelayTime::create(delay),
        FadeOut::create(1.0f),
        nullptr));
}

void CUIReplayBattleMain::updateTime()
{
    // ����ʱ��ֵ
    int leftSecond = (m_Helper->MaxTick - m_Helper->CurTick) / m_Helper->TickPerSecond;
    if (leftSecond < 0)
    {
        leftSecond = 0;
    }

    int min = leftSecond / 60;
    int second = leftSecond % 60;
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d", min, second);
    m_TimeText->setString(buf);

    // ʱ��С��30�뿪ʼ���棬playһ�Σ�ѭ������
    if (leftSecond == 30)
    {
        m_TimeAct->play("RedBlink", true);
    }
    // ʱ��С��10�뿪ʼ����ʱ��playһ�Σ�ѭ��
    else if (leftSecond == 10)
    {
        m_DownTimeNode->setVisible(true);
        playCsbAnimation(m_DownTimeNode, "Countdown", true);
        m_DownTimeText->setString(toolToStr(leftSecond));
    }
    else if (leftSecond < 10)
    {
        m_DownTimeText->setString(toolToStr(leftSecond));
    }

    playDownTimeEffect(leftSecond);
    changeBackgroudMood();
}

void CUIReplayBattleMain::updateHurt()
{
    if (m_Helper->getSettleAccountModel())
    {
        if (m_Helper->getSettleAccountModel()->getHitBossHP() != m_nHurt)
        {
            m_nHurt = m_Helper->getSettleAccountModel()->getHitBossHP();
            playCsbAnimation(m_HurtNode, "HurtsAdd");
            m_HurtText->setString(toolToStr(m_nHurt));
        }
    }
}

void CUIReplayBattleMain::updateDeath()
{
    if (m_bUnDoDeathCamera)
    {
        // ��ȡ�ٻ�ʦ����
        auto hero = dynamic_cast<CRole*>(m_Helper->getMainRole(
            m_Helper->getCampWithUid(m_Helper->getUserId())));
        // ��ȡ�Է��ٻ�ʦ/boss
        auto enemy = dynamic_cast<CRole*>(m_Helper->getMainRole(
            m_Helper->getEnmeyCampWithUid(m_Helper->getUserId())));

        // �ж��Ƿ��������
        bool heroDie = hero->isRealDead();
        bool bossDie = enemy->isRealDead();
        if (heroDie || bossDie)
        {
            m_BattleUI->setVisible(false);
            m_bUnDoDeathCamera = false;
            // ��ͣ����
            CUIEffectManager::getInstance()->stopUIEffect();
            CBattleDragLayer *pDragLayer = dynamic_cast<CBattleDragLayer *>(
				Director::getInstance()->getRunningScene()->getChildByName("ScaleLayer")->getChildByTag(BLT_DRAG));
            CBattleLayer *pBattleLayer = dynamic_cast<CBattleLayer *>(
				Director::getInstance()->getRunningScene()->getChildByName("ScaleLayer")->getChildByTag(BLT_BATTLE));

            if (pDragLayer && pBattleLayer)
            {
                Size s = Director::getInstance()->getWinSize();
                // �������Ҫ������Ļ�Ĵ�С����ȫ����ס, ����4����Ļ
                // ������ɫ���
                LayerColor* pMaskLayer = LayerColor::create(cocos2d::Color4B(0, 0, 0, 200), s.width * 4, s.height * 4);
                pBattleLayer->addChild(pMaskLayer, SLZ_BATTLE2 + 1);
                pMaskLayer->setPosition(-s.width, -s.height);
                hero->setLocalZOrder(SLZ_BATTLE2 + 2);
                enemy->setLocalZOrder(SLZ_BATTLE2 + 2);

                // ������Ϸ�ٶ�
                Director::getInstance()->getScheduler()->setTimeScale(0.3f);
                pDragLayer->startCamera(heroDie ? 3 : 4);

                //���ûص�
                pDragLayer->setCameraFinishCallback([this, pMaskLayer](){
                    Director::getInstance()->getScheduler()->setTimeScale(1.f);
                    pMaskLayer->removeFromParent();
                });
            }
        }
    }
}

void CUIReplayBattleMain::playDownTimeEffect(int time)
{
    if (m_nDownTime == time)
    {
        return;
    }

    m_nDownTime = time;
    switch (time)
    {
    case 10:
    case 9:
    case 8:
    case 7:
    case 6:
    case 1:
        playSoundEffect(43);
        break;
    case 5:
    case 4:
    case 3:
    case 2:
        playSoundEffect(44);
        break;
    case 0:
        playSoundEffect(45);
        break;
    default:
        break;
    }
}

void CUIReplayBattleMain::changeBackgroudMood()
{
    const StageConfItem* pStageConfItem = queryConfStage(m_Helper->getStageId());
    CHECK_RETURN_VOID(NULL != pStageConfItem);
    const StageSceneConfItem* item = queryConfStageScene(pStageConfItem->StageSenceID);
    CHECK_RETURN_VOID(NULL != item);

    if (1.0f > m_fMoodParam && checkMoodCondition(item->IMSControl1))
    {
        m_fMoodParam = 1.0f;
        CFMODAudioEngine::getInstance()->setBackgroundMusicParam("mood", m_fMoodParam);
    }

    if (2.0f > m_fMoodParam && checkMoodCondition(item->IMSControl2))
    {
        m_fMoodParam = 2.0f;
        CFMODAudioEngine::getInstance()->setBackgroundMusicParam("mood", m_fMoodParam);
    }
}

bool CUIReplayBattleMain::checkMoodCondition(const VecVecInt& conditionList)
{
    bool bRlt = false;
    for (auto &condition : conditionList)
    {
        if (2 != condition.size())
        {
            bRlt = false;
            break;
        }

        if (1 == condition[0])
            bRlt = timeCondition(condition[1]);
        else if (2 == condition[0])
            bRlt = soilderCondition(condition[1]);
        else
            bRlt = crystalCondition(condition[1]);

        if (!bRlt)
            break;
    }

    return bRlt;
}

bool CUIReplayBattleMain::timeCondition(float time)
{
    float s = m_Helper->CurTick / m_Helper->TickPerSecond;
    
    return time <= s;
}

bool CUIReplayBattleMain::soilderCondition(int count)
{
    int c = m_Helper->getRoleWithCamp(CampType::ECamp_Blue).size()
        + m_Helper->getEnmeyRoleWithCamp(CampType::ECamp_Blue).size();

    return count <= c;
}

bool CUIReplayBattleMain::crystalCondition(int level)
{
    CRole *pPlayerHero = m_Helper->getMainRole(m_Helper->getCampWithUid(m_Helper->getUserId()));
    CHECK_RETURN(NULL != pPlayerHero);

    return pPlayerHero->getIntAttribute(EHeroCrystalLevel) >= level;
}

void CUIReplayBattleMain::onBtnAdd(Ref* object)
{
	CBattleDragLayer *pDragLayer = dynamic_cast<CBattleDragLayer *>(Director::getInstance()->getRunningScene()->getChildByName("ScaleLayer")->getChildByTag(BLT_DRAG));
	pDragLayer->testScaleZoomOut(pDragLayer);
}

void CUIReplayBattleMain::onBtnMinus(Ref* object)
{
	CBattleDragLayer *pDragLayer = dynamic_cast<CBattleDragLayer *>(Director::getInstance()->getRunningScene()->getChildByName("ScaleLayer")->getChildByTag(BLT_DRAG));
	pDragLayer->testScaleZoomIn(pDragLayer);
}

void CUIReplayBattleMain::onBtnSpeed(Ref* object)
{
	if (m_nSpeed == 1)
	{
		m_nSpeed = 2;
		Director::getInstance()->getScheduler()->setTimeScale(m_nSpeed);
        m_SpeedBtnAct->setLastFrameCallFunc([=](){m_SpeedBtnAct->play("X2", true); });
        m_SpeedBtnAct->play("X1ToX2", false);
	}
	else if (m_nSpeed == 2)
	{
		m_nSpeed = 1;
		Director::getInstance()->getScheduler()->setTimeScale(m_nSpeed);
        m_SpeedBtnAct->setLastFrameCallFunc([=](){m_SpeedBtnAct->play("X1", false); });
        m_SpeedBtnAct->play("X2ToX1", false);
	}
}

void CUIReplayBattleMain::setSummonerHeroInfo(bool isLeft)
{
    std::string s = isLeft ? "L" : "R";
    // ��ʼ��������ƺ͹�������
    auto userName = findChild<Text>(m_BattleUI, string("MainPanel/DownPanel_" + s + "/UserName").c_str());
    auto unionName = findChild<Text>(m_BattleUI, string("MainPanel/DownPanel_" + s + "/GuildName").c_str());
    userName->setString(s);
    unionName->setString("(" + s + ")");

    // ��ȡ�ٻ�ʦ����
    auto hero = dynamic_cast<CHero*>(m_Helper->getMainRole(
        m_Helper->getCampWithUid(isLeft ? m_Helper->getUserId() : m_Helper->getEnmeyUserId())));
    CHECK_RETURN_VOID(nullptr != hero);

    // ��ʼ�������ٻ�ʦ�͵з��ٻ�ʦ������
    auto summonerPanel = findChild<Node>(m_BattleUI, string("MainPanel/ScreenPanel_3/SummonerPanel_" + s).c_str());
    auto summonerComponent = new CUISummonerComponent();
    summonerComponent->init(summonerPanel, hero, false);
    summonerPanel->addComponent(summonerComponent);
    summonerComponent->release();

    // ��ʼ��7��ʿ����Ƭ ���� �±�Ϊ0123456
    for (int i = 0; i < 7; ++i)
    {
        auto heroCard = findChild<Node>(m_BattleUI, string("MainPanel/DownPanel_" + s + "/HeroButton_" + toolToStr(i + 1) + "/HeroItem").c_str());
        if (hero->getSoldierCard(i) != nullptr)
        {
            auto heroComponent = new CUIReplayHeroCardComponent();
            heroComponent->init(heroCard, hero, i);
            heroCard->addComponent(heroComponent);
            heroComponent->release();
        }
        else
        {
            // ��ʼ����ͼ��
            auto rootNode = CSLoader::createNode("ui_new/f_fight/Fight_HeroItem_Null.csb");
            rootNode->setPosition(heroCard->getPosition());
            rootNode->setLocalZOrder(heroCard->getLocalZOrder());
            heroCard->getParent()->addChild(rootNode);
            heroCard->removeFromParentAndCleanup(true);
        }
    }

    // ��ʼ��Ӣ�ۼ��� ���� �±�Ϊ123
    for (int i = 1; i < 4; ++i)
    {
        auto skillBtn = findChild<Button>(m_BattleUI, string("MainPanel/DownPanel_" + s + "/SkillButton_" + toolToStr(i)).c_str());
        auto skillNode = findChild<Node>(skillBtn, "SkillItem");
        auto skillComponent = new CUIReplaySkillComponent();
        skillComponent->init(skillNode, hero, i);
        skillNode->addComponent(skillComponent);
        skillComponent->release();
    }

    // ��ʼ��ˮ��
    auto crytalBtn = findChild<Node>(m_BattleUI, string("MainPanel/DownPanel_"+ s +"/GemButton/GemItem").c_str());
    auto crytalComponent = new CUIReplayCrystalComponent();
    crytalComponent->init(crytalBtn, hero);
    crytalBtn->addComponent(crytalComponent);
    crytalComponent->release();
}
//////////////////////////////////////////////////////////////////////////
static int index = 0;
void CUIReplayBattleMain::hero(Ref* ref)
{
    BattleCommandInfo cmd;
    cmd.CommandId = CommandSummoner;
    cmd.ExecuterId = CGame::getInstance()->BattleHelper->getUserId();
    cmd.Tick = CGame::getInstance()->BattleHelper->GameTick + 6;
    cmd.Ext1 = (index++)%7;
    cmd.Ext2 = 0;
    CGame::getInstance()->sendRequest(CMD_BATTLE, CMD_BAT_PVPCOMMANDCS, &cmd, sizeof(cmd));
}

void CUIReplayBattleMain::test()
{
    Size winSize = Director::getInstance()->getVisibleSize();

    MenuItemFont* crystal = MenuItemFont::create(
        "crystal",
        CC_CALLBACK_1(CUIReplayBattleMain::crystal, this)
        );
    crystal->setPosition(winSize.width - 100, winSize.height - 70);

    MenuItemFont* skill = MenuItemFont::create(
        "skill",
        CC_CALLBACK_1(CUIReplayBattleMain::skill, this)
        );
    skill->setPosition(winSize.width - 100, winSize.height - 110);

    MenuItemFont* hero = MenuItemFont::create(
        "hero",
        CC_CALLBACK_1(CUIReplayBattleMain::hero, this)
        );
    hero->setPosition(winSize.width - 100, winSize.height - 150);

    auto menu = cocos2d::Menu::create(crystal, skill, hero, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 100);
}

void CUIReplayBattleMain::skill(Ref* ref)
{
    BattleCommandInfo cmd;
    cmd.CommandId = CommandSkill;
    cmd.ExecuterId = CGame::getInstance()->BattleHelper->getUserId();
    cmd.Tick = CGame::getInstance()->BattleHelper->GameTick + 6;
    cmd.Ext1 = 1;
    cmd.Ext2 = -1;
    CGame::getInstance()->sendRequest(CMD_BATTLE, CMD_BAT_PVPCOMMANDCS, &cmd, sizeof(cmd));
}

void CUIReplayBattleMain::crystal(Ref* ref)
{
    BattleCommandInfo cmd;
    cmd.CommandId = CommandCrystal;
    cmd.ExecuterId = CGame::getInstance()->BattleHelper->getUserId();
    cmd.Tick = CGame::getInstance()->BattleHelper->GameTick + 6;
    CGame::getInstance()->sendRequest(CMD_BATTLE, CMD_BAT_PVPCOMMANDCS, &cmd, sizeof(cmd));
}