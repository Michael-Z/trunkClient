#include "UICrystal.h"
#include "Game.h"
#include "Protocol.h"
#include "BattleProtocol.h"
#include "Hero.h"
#include "DisplayCommon.h"
#include "Events.h"
#include "ConfOther.h"
#include "ConfLanguage.h"
#include "LuaSummonerBase.h"

USING_NS_CC;
USING_NS_TIMELINE;
using namespace ui;
using namespace std;

float delayTime = 0.0f;

CUICrystalComponent::CUICrystalComponent()
{
	
}


CUICrystalComponent::~CUICrystalComponent()
{
}

bool CUICrystalComponent::init(cocos2d::Node* crystalNode, CHero* hero)
{
    bool ret = Component::init();
    setName("UICrystalComponent");
    crystalNode->scheduleUpdate();

    m_Hero = hero;
    m_CurLevel = m_Hero->getIntAttribute(EHeroCrystalLevel);
    m_CurCrystal = m_Hero->getFloatAttribute(EHeroCrystal);
    m_DefaultSpeed = m_Hero->getFloatAttribute(EHeroCrystalSpeedParam);
    m_Price = queryConfCrystal(m_CurLevel)->Price;

    m_bEffectPlaying = false;
    m_CrystalAnimation = getCsbAnimation(crystalNode);
    m_CrystalAnimation->setLastFrameCallFunc([this]{
        m_bEffectPlaying = false;   // �����Ч�������
    });
    // ˮ������������涯���б�Add���桢Minus���桢Normal����
    m_TextAnimation = getCsbAnimation(findChild<Node>(crystalNode, "GemPanel/UpLvGemNum"));
    m_AddGemAnimation = getCsbAnimation(findChild<Node>(crystalNode, "LevelPanel/AddGem"));
    m_AddGemAnimation->play("Hide", false);

    m_bLevelUpTips = true;

    m_LevelText = findChild<Text>(crystalNode, "LevelPanel/LvText");
    m_CurCrystalText = findChild<Text>(crystalNode, "LevelPanel/GemNum");
    m_MaxCrystalText = findChild<Text>(crystalNode, "LevelPanel/GemSum");
    m_ClimbTowerText = findChild<Text>(crystalNode, "LevelPanel/ClimbTowerGemNum");
    m_AddGemText = findChild<Text>(crystalNode, "LevelPanel/AddGem/NumPanel/GemNum");
    m_PriceText = findChild<Text>(crystalNode, "GemPanel/UpLvGemNum/HeroItem/LabelColor/GemLabel");
    m_LoadingBar = findChild<LoadingBar>(crystalNode, "LevelPanel/LoadingBar_1");

    cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("ui_new/f_fight/effect/GemState.ExportJson");
    m_BuffAnimation = Armature::create("GemState");
    m_BuffAnimation->setVisible(false);
    m_BuffAnimation->setPosition(Vec2(112.0f, 24.0f));
    m_BuffAnimation->setLocalZOrder(CUICrystalComponent::LZ_CRYSTALSTATE);
    m_LevelText->getParent()->addChild(m_BuffAnimation);

    m_TipsFinger = findChild<Node>(crystalNode, "TipsFinger");
    m_TipsFingerAnimation = getCsbAnimation(m_TipsFinger);

    // �����������ܵ�ˮ��
    if (m_Hero->getBattleHelper()->getBattleType() == EBATTLE_TOWERTEST)
    {
        m_CrystalAnimation->play("Close", false);       // �ر�ˮ��

        m_MaxCrystal = m_CurCrystal;
        m_ClimbTowerText->setVisible(true);
        setClimbTowerText();
        setLoadingBarPercent();


        m_LevelText->setVisible(false);
        m_CurCrystalText->setVisible(false);
        m_MaxCrystalText->setVisible(false);
        m_PriceText->setVisible(false);
        m_TipsFinger->setVisible(false);
    }
    else
    {
        m_MaxCrystal = queryConfCrystal(m_CurLevel)->Max;

        setCurLevelText();
        setCurCrystalText();
        setMaxCrystalText();
        setPriceText();

        dynamic_cast<Button*>(crystalNode->getParent())->addClickEventListener(
            CC_CALLBACK_1(CUICrystalComponent::onClick, this));
    }

    return ret;
}

void CUICrystalComponent::update(float dt)
{
    // �����������ܵ�ˮ��
    if (m_Hero->getBattleHelper()->getBattleType() == EBATTLE_TOWERTEST)
    {
        float curCrystal = m_Hero->getFloatAttribute(EHeroCrystal);
        if (m_CurCrystal != curCrystal)
        {
            setCurCrystal();
            setClimbTowerText();
            setLoadingBarPercent();
        }
    }
    else
    {
        playCrystalEffect(dt);
        playBuffEffect();
    }
}


/*
@brief  ����ˮ���ȼ�
*/
void CUICrystalComponent::setCurLevel()
{
    m_CurLevel = m_Hero->getIntAttribute(EHeroCrystalLevel);
}

/*
@brief  ���õ�ǰ�����۸�
*/
void CUICrystalComponent::setPrice()
{
    m_Price = queryConfCrystal(m_CurLevel)->Price;
}

/*
@brief  ���õ�ǰˮ������
*/
void CUICrystalComponent::setCurCrystal()
{
    m_CurCrystal = m_Hero->getFloatAttribute(EHeroCrystal);
}

/*
@brief  �������ˮ������
*/
void CUICrystalComponent::setMaxCrystal()
{
    m_MaxCrystal = queryConfCrystal(m_CurLevel)->Max;
}

/*
@brief  ����ˮ���ȼ��ı�
*/
void CUICrystalComponent::setCurLevelText()
{
    if (m_Hero->isCrystalMaxLevel())
    {
        m_LevelText->setString(getLanguageString(CONF_UI_LAN, 578));
    }
    else
    {
        //m_LevelText->setString("Lv." + toolToStr(m_CurLevel));
        m_LevelText->setString(toolToStr(m_CurLevel));
    }
}

/*
@brief  ���õ�ǰˮ�������ı�
*/
void CUICrystalComponent::setCurCrystalText()
{
    m_CurCrystalText->setString(toolToStr(static_cast<int>(m_CurCrystal)));
}

/*
@brief  �������ˮ�������ı�
*/
void CUICrystalComponent::setMaxCrystalText()
{
    m_MaxCrystalText->setString(std::string("/") + toolToStr(m_MaxCrystal));
}

/*
* @brief  ������������ˮ�������ı�
*/
void CUICrystalComponent::setClimbTowerText()
{
    m_ClimbTowerText->setString(toolToStr(static_cast<int>(m_CurCrystal)));
}

/*
* @brief  ���õ�ǰ�����۸��ı�
*/
void CUICrystalComponent::setPriceText()
{
    if (m_Price <= 0)
    {
        m_PriceText->setString("---");
        m_PriceText->setTextColor(Color4B::WHITE);
    }
    else
    {
        m_PriceText->setString(toolToStr(m_Price));
        if (m_Hero->canUseCrystal(m_Price))
        {
            if (m_DefaultSpeed == 1)
            {
                // ����״̬
                m_PriceText->setTextColor(Color4B::WHITE);
            }
            else if (m_DefaultSpeed < 1)
            {
                // ˮ���ָ�����
                m_PriceText->setTextColor(Color4B::YELLOW);
            }
            else if (m_DefaultSpeed > 1)
            {
                // ˮ���ָ�����
                m_PriceText->setTextColor(Color4B::GREEN);
            }
        }
        else
        {
            // ˮ������
            m_PriceText->setTextColor(Color4B::RED);
        }
    }
}

/*
* @brief   ����ˮ�����ؽ�����
*/
void CUICrystalComponent::setLoadingBarPercent()
{
    m_LoadingBar->setPercent(getLoadPercent());
}

/**
@brief  ����ˮ����״̬��Ч
*/
void CUICrystalComponent::playCrystalEffect(float dt)
{
    float curCrystal = m_Hero->getFloatAttribute(EHeroCrystal);
    if (m_CurCrystal != curCrystal)
    {
        setCurCrystal();
        setCurCrystalText();
    }
    if (m_CurLevel != m_Hero->getIntAttribute(EHeroCrystalLevel))
    {
        setCurLevel();
        setCurLevelText();
        setMaxCrystal();
        setMaxCrystalText();
        setPrice();
    }

    setPriceText();
    setLoadingBarPercent();

    // ˮ������
    if (!m_Hero->canUseCrystal(m_Price))
    {
        m_bLevelUpTips = true;
        if (!m_bEffectPlaying)
        {
            m_CrystalAnimation->play("Close", false);       // �ر�ˮ��
        }
        delayTime = 0.0f;
        m_bFingerTips = false;
        m_TipsFinger->setVisible(false);
    }
    // ˮ���ȼ���������δ�ﵽ���ֵ��ˮ����������
    else if (!m_Hero->isCrystalMaxLevel() && static_cast<int>(m_CurCrystal) < m_MaxCrystal && m_Hero->canUseCrystal(m_Price))
    {
        if (!m_bEffectPlaying && m_bLevelUpTips)
        {
            onLuaEvent(16);
            m_bLevelUpTips = false;
            playEffect("CanLevelUp");
        }
        else if (!m_bEffectPlaying && !m_bLevelUpTips)
        {
            playEffect("CanLevelUpLoop");
        }
        delayTime = 0.0f;
        m_bFingerTips = false;
        m_TipsFinger->setVisible(false);
    }
    // ˮ���ȼ�δ�ﵽ��������ﵽ���ֵ
    else if (!m_Hero->isCrystalMaxLevel() && static_cast<int>(m_CurCrystal) == m_MaxCrystal)
    {
        if (!m_bEffectPlaying)
        {
            playEffect("GemFull");
        }
        // ������ָ����
        if (!m_bFingerTips)
        {
            delayTime += dt;
            if (delayTime > 5.0f)
            {
                delayTime = 0.0f;
                m_bFingerTips = true;
                m_TipsFinger->setVisible(true);
                m_TipsFingerAnimation->play("Down3", true);
            }
        }
    }
    // ˮ���ȼ��ﵽ�������δ�ﵽ���ֵ
    else if (m_Hero->isCrystalMaxLevel() && static_cast<int>(m_CurCrystal) < m_MaxCrystal)
    {
        if (!m_bEffectPlaying)
        {
            m_bEffectPlaying = false;
            m_CrystalAnimation->play("MaxLevelUp", false);
        }
        delayTime = 0.0f;
        m_bFingerTips = false;
        m_TipsFinger->setVisible(false);
    }
    // ˮ���ȼ����������ﵽ���ֵ
    else if (m_Hero->isCrystalMaxLevel() && static_cast<int>(m_CurCrystal) == m_MaxCrystal)
    {
        if (!m_bEffectPlaying)
        {
            playEffect("LVGemFull");
        }
        delayTime = 0.0f;
        m_bFingerTips = false;
        m_TipsFinger->setVisible(false);
    }
}

/**
@brief  ����ˮ�������������Ч
*/
void CUICrystalComponent::playBuffEffect()
{
    // �����������Ч��
    if (m_DefaultSpeed == 1 || m_Hero->getFloatAttribute(EHeroCrystal) == m_MaxCrystal)
    {
        // ����״̬
        m_TextAnimation->play("Normal", false);
        m_BuffAnimation->setVisible(false);
    }
    else if (m_DefaultSpeed < 1 && m_Hero->getFloatAttribute(EHeroCrystal) < m_MaxCrystal)
    {
        // ˮ���ָ�����
        m_TextAnimation->play("Minus", true);
        if (!m_BuffAnimation->isVisible())
        {
            m_BuffAnimation->setVisible(true);
            m_BuffAnimation->getAnimation()->play("Minus", -1, 1);
        }
    }
    else if (m_DefaultSpeed > 1 && m_Hero->getFloatAttribute(EHeroCrystal) < m_MaxCrystal)
    {
        // ˮ���ָ�����
        m_TextAnimation->play("Add", true);
        if (!m_BuffAnimation->isVisible())
        {
            m_BuffAnimation->setVisible(true);
            m_BuffAnimation->getAnimation()->play("Add", -1, 1);
        }
    }
}

void CUICrystalComponent::playEffect(const char* effName)
{
    m_bEffectPlaying = true;
    m_CrystalAnimation->play(effName, false);
}

void CUICrystalComponent::onClick(cocos2d::Ref* object)
{
    if (m_Hero->isCrystalMaxLevel())
    {
        // ���Ų�����Ч��Ч
        playSoundEffect(6);
        return;
    }

    if (m_Hero->canUseCrystal(m_Price))
    {
        BattleCommandInfo cmd;
        cmd.CommandId = CommandCrystal;
        cmd.ExecuterId = CGame::getInstance()->BattleHelper->getUserId();
        cmd.Tick = CGame::getInstance()->BattleHelper->GameTick + 6;
        CGame::getInstance()->sendRequest(CMD_BATTLE, CMD_BAT_PVPCOMMANDCS, &cmd, sizeof(cmd));
        playEffect("LevelUp");
        playUISoundEffect(dynamic_cast<Node*>(object));
        onLuaEvent(5);

        // ����ˮ����Ԥ��ֵ
        m_Hero->setWithholdValue(m_Price);
    }
    else
    {
        // ���Ų�����Ч��Ч
        playSoundEffect(6);
        CGame::getInstance()->EventMgr->raiseEvent(BattleEventShowTips,
            (void*)(getLanguageString(CONF_UI_LAN, 215)));
        if ( !m_bEffectPlaying)
        {
            playEffect("OnNoGem");
        }
    }
}

/*
@brief  ��ȡ���صĽ���
*/
float CUICrystalComponent::getLoadPercent()
{
    if (m_MaxCrystal == 0)
    {
        return 0.0f;
    }
    else
    {
        return 100 * m_CurCrystal / m_MaxCrystal;
    }
}
