#include "UIHeroCard.h"
#include "Game.h"
#include "Protocol.h"
#include "BattleProtocol.h"
#include "Hero.h"
#include "DisplayCommon.h"
#include "Events.h"
#include "ConfOther.h"
#include "ConfLanguage.h"
#include "LuaSummonerBase.h"
#include "ConfGameSetting.h"
#include "ResPool.h"

USING_NS_CC;
USING_NS_TIMELINE;
using namespace ui;
using namespace std;

CUIHeroCardComponent::CUIHeroCardComponent()
: m_HeroOnly(NULL)
, m_LockAnimation(NULL)
, m_BuffAnimation(NULL)
, m_CurState(HeroCardState::HeroCardInvalid)
, m_iTouchingTime(0)
, m_bTouching(false)
{
}

CUIHeroCardComponent::~CUIHeroCardComponent()
{
}

bool CUIHeroCardComponent::init(cocos2d::Node* cardNode, CHero* hero, int index)
{
    bool ret = Component::init();
    setName("UIHeroCardComponent");
    cardNode->scheduleUpdate();

    // ��ʼ������
    m_Index = index;
    m_Hero = hero;
    m_Model = hero->getSoldierCard(index);
    m_bLock = m_Model->IsLock;
    m_DefaultCD = m_Model->MaxCD;
    m_DefautlCost = m_Model->CurCost;

    // ���ƶ����б�LoadOver(CD����)��On(����״̬���)��OnCD(CD״̬���)��OnNoGem(ˮ��������)��OnOnly(Ψһ�������ɷ����)
    m_bEffectPlaying = false;
    m_CardAnimation = getCsbAnimation(cardNode);
    m_CardAnimation->setLastFrameCallFunc([this]{
        m_bEffectPlaying = false;   // �����Ч�������
    });
    //
    string icon = m_Model->getConf()->Common.HeadIcon;
    int jobs = m_Model->getConf()->Common.Vocation;
    int race = m_Model->getConf()->Common.Race;
	int rare = m_Model->getConf()->Rare;

	std::string str[] = { "NULL", "White", "Green", "Blue", "Voilet", "Yellow" };
	m_LightAnimation = getCsbAnimation(findChild<Node>(cardNode, "HeroItem/RayLight"));
	m_LightAnimation->play(str[rare], true);

	// ���ƽڵ�
	auto pSoldierItem = queryConfSoldierStarSetting(m_Model->getStar());
	auto pRareItem = queryConfSoldierRareSetting(rare);
	auto pIconItem = queryConfIconSetting();
    //
    m_HeroItem = findChild<Layout>(cardNode, "HeroItem");
    m_HeroBg = findChild<Button>(cardNode, "HeroItem/HeroBg");
    m_HeroIcon = findChild<Button>(cardNode, "HeroItem/HeroIcon");
    Sprite* heroFrame = findChild<Sprite>(cardNode, "HeroItem/LvImage");
    Sprite* heroJobs = findChild<Sprite>(cardNode, "HeroItem/Profesion");
	Sprite* heroJobsBar = findChild<Sprite>(cardNode, "HeroItem/ProfesionBar");
    Sprite* heroRace = findChild<Sprite>(cardNode, "HeroItem/Race");
    m_HeroCost = findChild<Text>(cardNode, "HeroItem/GemNum");
    m_LoadingMask = findChild<Sprite>(cardNode, "HeroItem/LoadingMask");
    //
	m_HeroBg->loadTextureNormal(pRareItem->HeadboxBgRes, TextureResType::PLIST);
    m_HeroIcon->loadTextureNormal(icon, TextureResType::PLIST);
	heroFrame->setSpriteFrame(pRareItem->BigHeadboxRes);
	heroJobs->setSpriteFrame(pRareItem->JobsIcon[jobs - 1]);
	heroJobsBar->setSpriteFrame(pRareItem->JobBg);

    heroRace->setSpriteFrame(pIconItem.RaceIcon[race - 1]);
    m_HeroCost->setString(toolToStr(m_Model->CurCost));

    // Ψһ��ʶ(Ӣ������)
    if (m_Model->IsSingo)
    {
        auto rootNode = CSLoader::createNode("ui_new/f_fight/effect/Fight_HeroItem_Only.csb");
        rootNode->setPosition(Vec2(85.0f, 112.0f));
        rootNode->setLocalZOrder(LZ_SINGLE);
        m_HeroBg->getParent()->addChild(rootNode);
        ImageView* onlyImage = findChild<ImageView>(rootNode, "OnlyPanel/LevelImage");
		onlyImage->loadTexture(pRareItem->CircleboxRes, TextureResType::PLIST);
        m_HeroOnly = findChild<Text>(rootNode, "OnlyPanel/NumColor/Num");
        m_HeroOnly->setString("1");
    }

    //Ӷ����ʶ
    TextBMFont* mercenaryLogo = findChild<TextBMFont>(cardNode, "HeroItem/MercenaryLogo");
    mercenaryLogo->setVisible(false);
    if (m_Model->IsMercenary)
    {
        mercenaryLogo->setVisible(true);
        mercenaryLogo->setString(getLanguageString(CONF_UI_LAN, 2041));
    }

	m_bButton = dynamic_cast<Button*>(cardNode->getParent());
	m_bButton->addTouchEventListener(CC_CALLBACK_2(CUIHeroCardComponent::touchCallBack, this));

    return ret;
}

void CUIHeroCardComponent::touchCallBack(Ref* psender, cocos2d::ui::Widget::TouchEventType type)
{
	auto button = static_cast<Button*>(psender);
	switch (type)
	{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			//����������
			if (CGame::getInstance()->BattleHelper->getBattleType() == EBATTLE_CHAPTER)
			{
				this->m_iTouchingTime = 0;
				m_bTouching = true;
			}
			break;
		}
		case cocos2d::ui::Widget::TouchEventType::MOVED:
		{
			break;
		}
		case cocos2d::ui::Widget::TouchEventType::ENDED:
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
		{
			this->m_iTouchingTime = 0;
			m_bTouching = false;
			UITouchInfo* UItouchInfo = (UITouchInfo*)m_bButton->getChildByName("UI_TOUCH");
			if (UItouchInfo && UItouchInfo->getisOpen())
			{
				UItouchInfo->setVisible(false);
				UItouchInfo->setisOpen(false);
			}
			else if (type == cocos2d::ui::Widget::TouchEventType::ENDED && (!UItouchInfo || UItouchInfo && !UItouchInfo->getisOpen()))
			{
				onClick(button);
			}
		}
		default:
			break;
	}
}

void CUIHeroCardComponent::update(float dt)
{
	//����
	if (m_bTouching)
	{
		m_iTouchingTime++;
		if (m_iTouchingTime >= 30)
		{
			m_bTouching = false;
			CCLOG(" addd");
			UITouchInfo* UItouchInfo = nullptr;
			UItouchInfo= (UITouchInfo*)m_bButton->getChildByName("UI_TOUCH");
			if (UItouchInfo && !UItouchInfo->getisOpen())
			{
				UItouchInfo->setVisible(true);
				UItouchInfo->updateUI(TOUCH_UI_TYPE_HERO, m_Model);
				UItouchInfo->setisOpen(true);
			}
			else if (!UItouchInfo)
			{
				//������ϸ��Ϣ
				UItouchInfo = UITouchInfo::create();
				UItouchInfo->setName("UI_TOUCH");
				auto conSize = m_bButton->getContentSize();
				if (m_Index == 6)//last one
				{
					UItouchInfo->setPosition(Vec2(-conSize.width / 2, conSize.height));
				}
				else
				{
					UItouchInfo->setPosition(Vec2(conSize.width / 2, conSize.height));
				}
				UItouchInfo->updateUI(TOUCH_UI_TYPE_HERO, m_Model);
				UItouchInfo->setisOpen(true);
				m_bButton->addChild(UItouchInfo);
			}
		}
	}

    // ���״̬�仯
    HeroCardState state = checkModelState();
    if (state != m_CurState)
    {
        // CD��������ܻ��е���������ˮ�����㣩����״̬
        switch (state)
        {
        case HeroCardState::HeroCardNormal:
        case HeroCardState::HeroCardLack:
            if (m_CurState == HeroCardState::HeroCardCD)
            {
                playEffect("LoadOver");
            }
            break;
        default:
            break;
        }
        m_CurState = state;
    }

    // CD�л���ˮ������ʱ�ڵ���ɫΪ��ɫ
    if (m_Model->CurCD < m_Model->MaxCD || !m_Hero->canUseCrystal(m_Model->CurCost))
    {
        m_HeroItem->setColor(Color3B(144, 144, 144));
    }
    else
    {
        m_HeroItem->setColor(Color3B(255, 255, 255));
    }

    // CD
    if (m_Model->CurCD < m_Model->MaxCD)
    {
        m_LoadingMask->setScaleY(1 - m_Model->CurCD / m_Model->MaxCD);
        m_LoadingMask->setVisible(true);
    }
    else
    {
        m_LoadingMask->setVisible(false);
    }

    // ˮ��
    if (m_Hero->canUseCrystal(m_Model->CurCost))
    {
        m_HeroBg->setBright(true);
        m_HeroIcon->setBright(true);
        if (m_DefautlCost > m_Model->CurCost)
        {
            m_HeroCost->setTextColor(Color4B::GREEN);
        }
        else if (m_DefautlCost < m_Model->CurCost)
        {
            m_HeroCost->setTextColor(Color4B::YELLOW);
        }
        else
        {
            m_HeroCost->setTextColor(Color4B::WHITE);
        }
    }
    else
    {
        m_HeroBg->setBright(false);
        m_HeroIcon->setBright(false);
        m_HeroCost->setTextColor(Color4B::RED);
    }
    m_HeroCost->setString(toolToStr(m_Model->CurCost));

    // Ψһ
    if (m_Model->IsSingo)
    {
        if (m_Hero->isSoldierSingle(m_Index))
        {
            m_HeroOnly->setString("0");
            m_HeroOnly->setColor(Color3B::GRAY);
        }
        else
        {
            m_HeroOnly->setString("1");
            m_HeroOnly->setColor(Color3B::WHITE);
        }
    }

    // ��(����Ч��)
    if (m_bLock != m_Model->IsLock)
    {
        if (NULL == m_LockAnimation)
        {
            auto rootNode = CSLoader::createNode("ui_new/f_fight/effect/Fight_HeroItem_Lock.csb");
            rootNode->setPosition(Vec2(50.0f, 70.0f));
            rootNode->setLocalZOrder(CUIHeroCardComponent::LZ_LOCK);
            rootNode->setScale(0.9f);
            m_HeroBg->getParent()->addChild(rootNode);
            m_LockAnimation = CSLoader::createTimeline("ui_new/f_fight/effect/Fight_HeroItem_Lock.csb");
            rootNode->runAction(m_LockAnimation);
        }
        if (m_bLock)
        {
            m_LockAnimation->play("Unlock", false);
        }
        else
        {
            m_LockAnimation->play("Lock", false);
        }
        m_bLock = m_Model->IsLock;
    }

    // buff
    if (m_DefaultCD == m_Model->MaxCD)
    {
        if (m_BuffAnimation)
        {
            m_BuffAnimation->play("Null", false);
        }
    }
    else
    {
        if (NULL == m_BuffAnimation)
        {
            auto rootNode = CSLoader::createNode("ui_new/f_fight/effect/Fight_HeroItem_U_State.csb");
            rootNode->setPosition(Vec2(50.0f, 76.0f));
            rootNode->setLocalZOrder(CUIHeroCardComponent::LZ_BUFF);
            rootNode->setScale(0.84f);
            m_HeroBg->getParent()->addChild(rootNode);
            m_BuffAnimation = CSLoader::createTimeline("ui_new/f_fight/effect/Fight_HeroItem_U_State.csb");
            rootNode->runAction(m_BuffAnimation);
            m_BuffAnimation->play("Add", true);
        }
        if (m_DefaultCD > m_Model->MaxCD)
        {
            m_BuffAnimation->play("Add", true);
        }
        else if (m_DefaultCD < m_Model->MaxCD)
        {
            m_BuffAnimation->play("Minus", true);
        }
    }
}

void CUIHeroCardComponent::playEffect(const char* effName)
{
    m_bEffectPlaying = true;
    m_CardAnimation->play(effName, false);
}

void CUIHeroCardComponent::onClick(cocos2d::Ref* object)
{
    // ���Ʊ���ס
    if (m_Model->IsLock)
    {
        // ���Ų�����Ч��Ч
        playSoundEffect(6);
        if (m_LockAnimation)
        {
            m_LockAnimation->play("OnLock", false);
        }
    }
    else if (m_Hero->canUseSoldierCard(m_Index))
    {
        BattleCommandInfo cmd;
        cmd.CommandId = CommandSummoner;
        cmd.ExecuterId = CGame::getInstance()->BattleHelper->getUserId();
        cmd.Tick = CGame::getInstance()->BattleHelper->GameTick + 6;
        cmd.Ext1 = m_Index;
        cmd.Ext2 = 0;
		CGame::getInstance()->sendRequest(CMD_BATTLE, CMD_BAT_PVPCOMMANDCS, &cmd, sizeof(cmd));
        playEffect("On");
        playUISoundEffect(dynamic_cast<Node*>(object));

        // ����ˮ����Ԥ��ֵ
        CSoldierModel* soldierCard = m_Hero->getSoldierCard(m_Index);
        m_Hero->setWithholdValue(soldierCard->CurCost);
        onLuaEventWithParamInt(3, soldierCard->getSoldId());
    }
    else
    {
        // ���Ų�����Ч��Ч
        playSoundEffect(6);
        // ���ŵ��ʧ��Ч��
        switch (m_CurState)
        {
        case HeroCardState::HeroCardCD:
            {
                CGame::getInstance()->EventMgr->raiseEvent(BattleEventShowTips,
                    (void*)(getLanguageString(CONF_UI_LAN, 596)));
                playEffect("OnCD");
            }
            break;
        case HeroCardState::HeroCardLack:
            {
                CGame::getInstance()->EventMgr->raiseEvent(BattleEventShowTips,
                    (void*)(getLanguageString(CONF_UI_LAN, 215)));
                playEffect("OnNoGem");
            }
            break;
        default:
            if (m_Hero->isSoldierSingle(m_Index))
            {
                CGame::getInstance()->EventMgr->raiseEvent(BattleEventShowTips,
                    (void*)(getLanguageString(CONF_UI_LAN, 217)));
                playEffect("OnOnly");
            }
            break;
        }
    }
}

CUIHeroCardComponent::HeroCardState CUIHeroCardComponent::checkModelState()
{
    if (m_Model->CurCD < m_Model->MaxCD)
    {
        return HeroCardState::HeroCardCD;
    }
    else if (!m_Hero->canUseCrystal(m_Model->CurCost))
    {
        return HeroCardState::HeroCardLack;
    }
    else
    {
        return HeroCardState::HeroCardNormal;
    }
}
