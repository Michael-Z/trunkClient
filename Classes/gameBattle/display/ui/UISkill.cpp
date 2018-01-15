#include "UISkill.h"
#include "Game.h"
#include "Protocol.h"
#include "BattleProtocol.h"
#include "Hero.h"
#include "Skill.h"
#include "DisplayCommon.h"
#include "Events.h"
#include "ConfOther.h"
#include "ConfLanguage.h"
#include "LuaSummonerBase.h"
#include "UIEffect.h"
#include "UITouchInfo.h"

USING_NS_CC;
USING_NS_TIMELINE;
using namespace ui;
using namespace std;


CUISkillComponent::CUISkillComponent()
: m_CurState(SkillState::SkillInvalid)
, m_iTouchingTime(0)
, m_bTouching(false)
{
}

CUISkillComponent::~CUISkillComponent()
{
}

void CUISkillComponent::onExit()
{
    CGame::getInstance()->EventMgr->removeEventHandle(this);
}

bool CUISkillComponent::init(cocos2d::Node* skillNode, CHero* hero, int index)
{
    bool ret = Component::init();
    setName("UISkillComponent");
    skillNode->scheduleUpdate();

    m_Index = index;
    m_Hero = hero;
    m_bEffectPlaying = false;
	m_bWaitForClick = false;
    m_Skill = m_Hero->getSkillWithIndex(m_Index);

    m_SkillAnimation = getCsbAnimation(skillNode);
    m_SkillAnimation->setLastFrameCallFunc([this]{
        m_bEffectPlaying = false;       // �����Ч�������
    });

    // ��ʼ������ͼ��
    auto iconName = m_Skill->getSkillConf()->BattleSkillIcon;
    m_SkillIcon = findChild<Button>(skillNode, "FightSkillItem/SkillImage");
    m_SkillIcon->loadTextureNormal(iconName, TextureResType::PLIST);

    // ����
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui_new/p_public/p_public.plist");
    m_SkillMask = ProgressTimer::create(Sprite::createWithSpriteFrameName("circle_68X68.png"));
    m_SkillMask->setType(ProgressTimer::Type::RADIAL);
    m_SkillMask->setReverseProgress(false);
    m_SkillMask->setMidpoint(Vec2(0.5f, 0.5f));
    m_SkillMask->setPosition(Vec2(35.0f, 35.0f));
    m_SkillMask->setPercentage(0);
    m_SkillMask->setVisible(false);
    m_SkillIcon->getParent()->addChild(m_SkillMask, CUISkillComponent::LZ_SKILLMASK);

    // ע������¼�����
    CGame::getInstance()->EventMgr->addEventHandle(BattleEventTouchCancelSkill,
        this, CALLBACK_FUNCV(CUISkillComponent::onSkillCancel));
    CGame::getInstance()->EventMgr->addEventHandle(BattleEventTouchPlaySkill,
        this, CALLBACK_FUNCV(CUISkillComponent::onSkillPlay));

	m_bButton = dynamic_cast<Button*>(skillNode->getParent());
	m_bButton->addTouchEventListener(CC_CALLBACK_2(CUISkillComponent::touchCallBack, this));

    return ret;
}

void CUISkillComponent::touchCallBack(Ref* psender, cocos2d::ui::Widget::TouchEventType type)
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

void CUISkillComponent::update(float dt)
{
	//����
	if (m_bTouching)
	{
		m_iTouchingTime++;
		if (m_iTouchingTime >= 30)
		{
			m_bTouching = false;
			UITouchInfo* UItouchInfo = nullptr;
			UItouchInfo = (UITouchInfo*)m_bButton->getChildByName("UI_TOUCH");
			if (UItouchInfo && !UItouchInfo->getisOpen())
			{
				UItouchInfo->setVisible(true);
				UItouchInfo->updateUI(TOUCH_UI_TYPE_SKILL, m_Skill);
				UItouchInfo->setisOpen(true);
			}
			else if (!UItouchInfo)
			{
				UItouchInfo = UITouchInfo::create();
				UItouchInfo->setName("UI_TOUCH");
				auto conSize = m_bButton->getContentSize();
				UItouchInfo->setPosition(Vec2(conSize.width / 2, 0));
				UItouchInfo->setVisible(true);
				UItouchInfo->updateUI(TOUCH_UI_TYPE_SKILL, m_Skill);
				UItouchInfo->setisOpen(true);
				m_bButton->addChild(UItouchInfo);
			}
		}
	}

    SkillState state = checkSkillState();
    switch (state)
    {
    case SkillState::SkillLock:
        if (m_CurState != state)
        {
            m_bEffectPlaying = false;
            m_SkillAnimation->play("Lock", false);

            m_SkillIcon->setBright(false);
            m_SkillMask->setVisible(false);

            m_CurState = state;
        }
        break;
    case SkillState::SkillCD:
        if (m_CurState != state)
        {
            m_bEffectPlaying = false;
            m_SkillAnimation->play("Stop", false);

            m_SkillIcon->setBright(true);
            m_SkillMask->setPercentage(0.0f);
            m_SkillMask->setVisible(true);
            m_SkillMask->setReverseDirection(true);

            m_CurState = state;
        }
        m_SkillMask->setPercentage((1.0f - m_Skill->getCDPercent()) * 100.0f);
        break;
    case SkillState::SkillNormal:
        // ����״̬��ת������״̬: ���Ž�������
        if (m_CurState == SkillState::SkillLock)
        {
            auto delay = cocos2d::DelayTime::create(0.75f);
            auto callback = cocos2d::CallFunc::create([this]()
            {
                m_SkillIcon->setBright(true);
                playEffect("Unlock");
                playSoundEffect(46);
            });
            auto sequence = cocos2d::Sequence::create(delay, callback, nullptr);
            this->getOwner()->runAction(sequence);

            m_SkillMask->setVisible(false);
            m_CurState = state;
            m_bEffectPlaying = true;
        }
        else if(!m_bEffectPlaying && !m_bWaitForClick)
        {
            if (m_CurState != state)
            {
                m_SkillIcon->setBright(true);
                m_SkillMask->setVisible(false);

                m_CurState = state;
                onLuaEventWithParamInt(15, m_Index);
            }
            playEffect("Normal", true);
        }

        break;
    case SkillState::SkillLack:
        if (m_CurState != state)
        {
            // ��CD״̬��ת��ˮ������״̬: ���ż�����ɶ���
            if (m_CurState == SkillState::SkillCD)
            {
                playEffect("LoadOver");
            }

            m_SkillIcon->setBright(true);
            m_SkillMask->setVisible(false);
            
            m_CurState = state;
        }
        break;
    case SkillState::SkillExecuting:
        if (m_CurState != state)
        {
            m_bEffectPlaying = false;
            m_SkillAnimation->play("Stop", false);

            m_SkillIcon->setBright(true);
            m_SkillMask->setPercentage(0.0f);
            m_SkillMask->setVisible(true);
            m_SkillMask->setReverseDirection(false);
            
            m_CurState = state;
        }
        m_SkillMask->setPercentage(m_Hero->getCommonCDPercent() * 100.0f);
        break;
    default:
        break;
    }
}

void CUISkillComponent::playEffect(const char* effName, bool loop /*=false*/)
{
    m_bEffectPlaying = true;
    m_SkillAnimation->play(effName, loop);
}

void CUISkillComponent::onClick(cocos2d::Ref* object)
{
    // ���ܰ�ť�����
    // ������ڵȴ�ѡ��Ŀ���ʱ�����
    if (m_bWaitForClick)
    {
        // ȡ���ͷż��ܣ�on cancel
        m_bWaitForClick = false;
        CGame::getInstance()->EventMgr->raiseEvent(BattleEventTouchCancelSkill, &m_Index);
        // �رռ���Ŀ���ѡ����ʾ ���� BattleMain���߼�
        onLuaEventWithParamInt(6, m_Index);
    }
    else if (!m_Hero->isCommonCD() && m_Skill->canExecute())
    {
        if (m_Skill->getSkillConf()->CastType == SkillCastAtOnce)
        {
            BattleCommandInfo cmd;
            cmd.CommandId = CommandSkill;
            cmd.ExecuterId = CGame::getInstance()->BattleHelper->getUserId();
            cmd.Tick = CGame::getInstance()->BattleHelper->GameTick + 6;
            cmd.Ext1 = m_Index;
            cmd.Ext2 = -1;
			CGame::getInstance()->sendRequest(CMD_BATTLE, CMD_BAT_PVPCOMMANDCS, &cmd, sizeof(cmd));
            // ���ų�̬���Ч��
            playEffect("On");
            playUISoundEffect(dynamic_cast<Node*>(object));

            float time = m_Skill->getSkillConf()->MaxCast * m_Skill->getSkillConf()->CastTime;
            m_Hero->setMaxSkillExecutingTime(time);
        }
        else if (m_Skill->getSkillConf()->CastType == SkillCastAtPoint)
        {
            // ��������Ŀ���ѡ����ʾ ���� BattleMain���߼�
            m_bWaitForClick = true;
            CGame::getInstance()->EventMgr->raiseEvent(BattleEventTouchReleaseSkill, &m_Index);
            // ���ų�̬���Ч��
            playEffect("OnLoop", true);
        }

        onLuaEventWithParamInt(4, m_Index);
    }
    else if (m_CurState == SkillState::SkillLack)
    {
        CGame::getInstance()->EventMgr->raiseEvent(BattleEventShowTips,
            (void*)(getLanguageString(CONF_UI_LAN, 215)));
        // ����״̬�µ���޷�Ӧ
        playEffect("OnNoGem");
        // ���Ų�����Ч��Ч
        playSoundEffect(6);
    }
    else if (m_CurState == SkillState::SkillLock)
    {
        playEffect("OnLock");
        // ���Ų�����Ч��Ч
        playSoundEffect(6);
        CGame::getInstance()->EventMgr->raiseEvent(BattleEventShowTips,
            (void*)(getLanguageString(CONF_UI_LAN, 2020 + m_Index)));
    }
    else
    {
        // ���Ų�����Ч��Ч
        playSoundEffect(6);
    }
}

CUISkillComponent::SkillState CUISkillComponent::checkSkillState()
{
    if (m_Skill->isLock())
    {
        return SkillState::SkillLock;
    }
    else if (!m_Skill->isFinish() || (!m_Skill->isLock() && m_Hero->isCommonCD()))
    {
        // ���������ͷ�
        return SkillState::SkillExecuting;
    }
    else if (m_Skill->getCDPercent() < 1.0f)
    {
        // ��������CD
        return SkillState::SkillCD;
    }
    else if (!m_Hero->canUseCrystal(m_Skill->getSkillConf()->CostTypeParam))
    {
        // �������Ĳ���
        return SkillState::SkillLack;
    }
    else
    {
        // ����״̬
        return SkillState::SkillNormal;
    }
}

void CUISkillComponent::onSkillCancel(void* data)
{
    int skillIndex = *(reinterpret_cast<int*>(data));
    m_bWaitForClick = false;
    if (skillIndex == m_Index)
    {
        playEffect("Normal", true);
    }
}

void CUISkillComponent::onSkillPlay(void* data)
{
    int skillIndex = *(reinterpret_cast<int*>(data));
    m_bWaitForClick = false;
    if (skillIndex == m_Index)
    {
        playEffect("Off");
        float time = m_Skill->getSkillConf()->MaxCast * m_Skill->getSkillConf()->CastTime;
        m_Hero->setMaxSkillExecutingTime(time);
    }
}
