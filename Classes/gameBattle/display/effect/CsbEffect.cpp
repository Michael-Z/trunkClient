#include "CsbEffect.h"
#include "ResManager.h"
#include "DisplayCommon.h"
#include "ui/UIHelper.h"
#include "ResPool.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace timeline;
using namespace std;

CCsbEffect::CCsbEffect()
: m_pNode(NULL)
, m_pAction(NULL)
{
}

CCsbEffect::~CCsbEffect()
{
}

bool CCsbEffect::init(const std::string& path, bool isDoLayout)
{
    CHECK_RETURN(CEffect::init());
    m_pNode = CResPool::getInstance()->getCsbNode(path);
    CHECK_RETURN(m_pNode);
    m_pAction = dynamic_cast<ActionTimeline*>(m_pNode->getActionByTag(m_pNode->getTag()));
    CHECK_RETURN(m_pAction);
    m_Path = path;
    addChild(m_pNode);
    
    if (isDoLayout)
    {
        m_pNode->setContentSize(Director::getInstance()->getWinSize());
        ui::Helper::doLayout(m_pNode);
    }
    return true;
}

bool CCsbEffect::init(int dir, const EffectConfItem* conf, const string& path)
{
    CHECK_RETURN(CEffect::init(dir, conf));
    m_pNode = CResPool::getInstance()->getCsbNode(path);
    CHECK_RETURN(m_pNode);

    m_pAction = dynamic_cast<ActionTimeline*>(m_pNode->getActionByTag(m_pNode->getTag()));
    CHECK_RETURN(m_pAction);

    // �ж϶����Ƿ����
    CHECK_RETURN(m_pAction->IsAnimationInfoExists(m_pConf->AnimationName));
    m_Path = path;
    addChild(m_pNode);

    // ���������ѭ�����е���������������ɺ󵭳�
    if (m_pConf->Loop <= 0)
    {
        m_pAction->setLastFrameCallFunc([this](){
            runAction(Sequence::create(FadeOut::create(m_pConf->FadeOutTime),
                RemoveSelf::create(true),
                NULL));
        });
    }

    // ����ȫ��ZOrder
    if (m_pConf->ZOrderType == EffZOrderGlobal)
    {
        updateGrobleZOrder(m_pNode, m_pConf->ZOrder);
    }
    m_pAction->setTimeSpeed(m_pConf->AnimationSpeed);
    return true;
}

void CCsbEffect::onEnter()
{
    CEffect::onEnter();
    if (m_pConf)
    {
        m_pAction->play(m_pConf->AnimationName, m_pConf->Loop > 0);
    }
}

void CCsbEffect::onExit()
{
    CEffect::onExit();
    if (m_pNode != nullptr)
    {
        CResPool::getInstance()->freeCsbNode(m_Path, m_pNode);
        removeChild(m_pNode);
        m_pNode = nullptr;
    }
}

// ����ָ������
bool CCsbEffect::playAnimate(const std::string& animate)
{
    CHECK_RETURN(m_pAction);
    // ��鶯��
    CHECK_RETURN(m_pAction->IsAnimationInfoExists(animate));
    m_pAction->play(animate, false);
    return true;
}

// ����ָ�����������ڶ�����������Զ��Ƴ�
bool CCsbEffect::playAnimateAutoRemove(const std::string& animate)
{
    if (playAnimate(animate))
    {
        m_pAction->setLastFrameCallFunc([this](){
            removeFromParent();
        });
        return true;
    }
    return false;
}