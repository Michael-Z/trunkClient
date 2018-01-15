#include "UIReplayCrystal.h"
#include "Game.h"
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

CUIReplayCrystalComponent::CUIReplayCrystalComponent()
: m_CurLevel(0)
, m_DefaultSpeed(0.0f)
, m_CurCrystal(0.0f)
, m_MaxCrystal(0)
, m_bEffectPlaying(false)
, m_Hero(nullptr)
, m_LevelText(nullptr)
, m_CurCrystalText(nullptr)
, m_MaxCrystalText(nullptr)
, m_AddGemText(nullptr)
, m_LoadingBar(nullptr)
, m_CrystalAnimation(nullptr)
, m_AddGemAnimation(nullptr)
, m_BuffAnimation(nullptr)
{
	
}

CUIReplayCrystalComponent::~CUIReplayCrystalComponent()
{
}

bool CUIReplayCrystalComponent::init(cocos2d::Node* crystalNode, CHero* hero)
{
    bool ret = Component::init();
    setName("UICrystalComponent");

    m_Hero = hero;
    m_CurLevel = m_Hero->getIntAttribute(EHeroCrystalLevel);
    m_CurCrystal = m_Hero->getFloatAttribute(EHeroCrystal); 
    m_DefaultSpeed = m_Hero->getFloatAttribute(EHeroCrystalSpeedParam);

    m_CrystalAnimation = getCsbAnimation(crystalNode);
    m_CrystalAnimation->setLastFrameCallFunc([this]{
        m_bEffectPlaying = false;   // �����Ч�������
    });

    // ˮ������������涯���б�Add���桢Minus���桢Normal����
    m_AddGemAnimation = getCsbAnimation(findChild<Node>(crystalNode, "LevelPanel/AddGem"));
    m_AddGemAnimation->play("Hide", false);

    m_LevelText = findChild<Text>(crystalNode, "LevelPanel/LvText");
    m_CurCrystalText = findChild<Text>(crystalNode, "LevelPanel/GemNum");
    m_MaxCrystalText = findChild<Text>(crystalNode, "LevelPanel/GemSum");
    m_AddGemText = findChild<Text>(crystalNode, "LevelPanel/AddGem/NumPanel/GemNum");
    m_LoadingBar = findChild<LoadingBar>(crystalNode, "LevelPanel/LoadingBar_1");

    cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("ui_new/f_fight/effect/GemState.ExportJson");
    m_BuffAnimation = Armature::create("GemState");
    m_BuffAnimation->setVisible(false);
    m_BuffAnimation->setPosition(Vec2(112.0f, 24.0f));
    m_BuffAnimation->setLocalZOrder(CUIReplayCrystalComponent::LZ_CRYSTALSTATE);
    m_LevelText->getParent()->addChild(m_BuffAnimation);

    m_MaxCrystal = queryConfCrystal(m_CurLevel)->Max;

    setCurLevelText();
    setCurCrystalText();
    setMaxCrystalText();

    // ע�����������¼�
    auto helper = CGame::getInstance()->BattleHelper;
    helper->pEventManager->addEventHandle(BattleEventCrystalUpgrade,
        this, CALLBACK_FUNCV(CUIReplayCrystalComponent::onCrystalUpgrade));

    return ret;
}

void CUIReplayCrystalComponent::update(float dt)
{
    playCrystalEffect(dt);
    playBuffEffect();
}

/*
@brief  ����ˮ���ȼ�
*/
void CUIReplayCrystalComponent::setCurLevel()
{
    m_CurLevel = m_Hero->getIntAttribute(EHeroCrystalLevel);
}

/*
@brief  ���õ�ǰˮ������
*/
void CUIReplayCrystalComponent::setCurCrystal()
{
    m_CurCrystal = m_Hero->getFloatAttribute(EHeroCrystal);
}

/*
@brief  �������ˮ������
*/
void CUIReplayCrystalComponent::setMaxCrystal()
{
    m_MaxCrystal = queryConfCrystal(m_CurLevel)->Max;
}

/*
@brief  ����ˮ���ȼ��ı�
*/
void CUIReplayCrystalComponent::setCurLevelText()
{
    if (m_Hero->isCrystalMaxLevel())
    {
        m_LevelText->setString(getLanguageString(CONF_UI_LAN, 578));
    }
    else
    {
        m_LevelText->setString(toolToStr(m_CurLevel));
    }
}

/*
@brief  ���õ�ǰˮ�������ı�
*/
void CUIReplayCrystalComponent::setCurCrystalText()
{
    m_CurCrystalText->setString(toolToStr(static_cast<int>(m_CurCrystal)));
}

/*
@brief  �������ˮ�������ı�
*/
void CUIReplayCrystalComponent::setMaxCrystalText()
{
    m_MaxCrystalText->setString(std::string("/") + toolToStr(m_MaxCrystal));
}

/*
* @brief   ����ˮ�����ؽ�����
*/
void CUIReplayCrystalComponent::setLoadingBarPercent()
{
    m_LoadingBar->setPercent(getLoadPercent());
}

void CUIReplayCrystalComponent::playCrystalEffect(float dt)
{
    float curCrystal = m_Hero->getFloatAttribute(EHeroCrystal);
    if (m_CurCrystal != curCrystal)
    {
        setCurCrystal();
        setCurCrystalText();
    }

    int crystalLevel = m_Hero->getIntAttribute(EHeroCrystalLevel);
    if (m_CurLevel != crystalLevel)
    {
        setCurLevel();
        setCurLevelText();
        setMaxCrystal();
        setMaxCrystalText();
    }

    setLoadingBarPercent();

    int maxCrystal = queryConfCrystal(crystalLevel)->Max;
    // ˮ���ȼ�δ�ﵽ��������ﵽ���ֵ
    if (!m_Hero->isCrystalMaxLevel() && static_cast<int>(curCrystal) == maxCrystal)
    {
        playEffect("LVGemFull");
    }
    // ˮ���ȼ��ﵽ�������δ�ﵽ���ֵ
    else if (m_Hero->isCrystalMaxLevel() && static_cast<int>(curCrystal) < maxCrystal)
    {
        playEffect("MaxLevelUp");
    }
    // ˮ���ȼ����������ﵽ���ֵ
    else if (m_Hero->isCrystalMaxLevel() && static_cast<int>(curCrystal) == maxCrystal)
    {
        playEffect("GemFull");
    }
}

/**
@brief  ����ˮ�������������Ч
*/
void CUIReplayCrystalComponent::playBuffEffect()
{
    // �����������Ч��
    if (m_DefaultSpeed == 1 || m_Hero->getFloatAttribute(EHeroCrystal) == m_MaxCrystal)
    {
        // ����״̬
        m_BuffAnimation->setVisible(false);
    }
    else if (m_DefaultSpeed < 1 && m_Hero->getFloatAttribute(EHeroCrystal) < m_MaxCrystal)
    {
        // ˮ���ָ�����
        if (!m_BuffAnimation->isVisible())
        {
            m_BuffAnimation->setVisible(true);
            m_BuffAnimation->getAnimation()->play("Minus", -1, 1);
        }
    }
    else if (m_DefaultSpeed > 1 && m_Hero->getFloatAttribute(EHeroCrystal) < m_MaxCrystal)
    {
        // ˮ���ָ�����
        if (!m_BuffAnimation->isVisible())
        {
            m_BuffAnimation->setVisible(true);
            m_BuffAnimation->getAnimation()->play("Add", -1, 1);
        }
    }
}

void CUIReplayCrystalComponent::onCrystalUpgrade(void* data)
{
    playEffect("LevelUp");
}

void CUIReplayCrystalComponent::playEffect(const char* effName)
{
    if (m_bEffectPlaying)
        return;

    m_bEffectPlaying = true;
    m_CrystalAnimation->play(effName, false);
}

/*
@brief  ��ȡ���صĽ���
*/
float CUIReplayCrystalComponent::getLoadPercent()
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