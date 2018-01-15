#include "UIEffect.h"
#include "GameComm.h"
#include "ConfGuide.h"
#include "Shake.h"
#include "Game.h"
#include "BattleDragLayer.h"
#include "EffectFactory.h"

#define SHAKE_TAG   8888

using namespace cocos2d;
using namespace cocostudio;
using namespace timeline;

CUIEffectManager* CUIEffectManager::m_Instance = nullptr;
CUIEffectManager* CUIEffectManager::getInstance()
{
    if (!m_Instance)
    {
        m_Instance = new CUIEffectManager;
    }
    return m_Instance;
}

void CUIEffectManager::destroy()
{
    if (m_Instance != NULL)
    {
        delete m_Instance;
        m_Instance = NULL;
    }
}

CUIEffectManager::CUIEffectManager()
: m_DarkNode(NULL)
, m_BackGround(NULL)
, m_Blink(NULL)
, m_Scene(NULL)
{

}

void CUIEffectManager::init(cocos2d::Node* battleScene)
{
    if (battleScene && !m_Scene)
    {
        Size s = Director::getInstance()->getWinSize();
        // �������Ҫ������Ļ�Ĵ�С����ȫ����ס, ����4����Ļ
        m_DarkNode = LayerColor::create(Color4B::BLACK, s.width*4, s.height*4);
        m_DarkNode->setPosition(-s.width, -s.height);
        SAFE_RETAIN(m_DarkNode);

        m_BackGround = NULL;
        m_Blink = NULL;
        m_Scene = battleScene;
        findDoodads(m_Scene);
    }
}

void CUIEffectManager::uninit()
{
    m_Scene = NULL;
    CC_SAFE_RELEASE_NULL(m_DarkNode);
    CC_SAFE_RELEASE_NULL(m_BackGround);
    CC_SAFE_RELEASE_NULL(m_Blink);
    releaseAndClearList(m_Doodads);
}

void CUIEffectManager::execute(int id, const Vec2& pos)
{
    auto conf = dynamic_cast<CConfUIEffect*>(CConfManager::getInstance()->getConf(CONF_UI_EFFECT));
    CHECK_RETURN_VOID(conf);
    auto item = static_cast<UIEffectConfItem*>(conf->getData(id));
    CHECK_RETURN_VOID(item);

    if (item->DarkTime > 0.0f)
    {
        // ִ�б䰵�߼�
        executeDark(item);
    }

    if (item->ShakeTime > 0.0f && item->ShakeLevel > 0)
    {
        // ִ�������߼�
        executeShake(item->ShakeLevel, item->ShakeDelayTime, item->ShakeTime);
    }

    if (item->BackgroundResID != 0)
    {
        // ��ӱ���
        if (m_BackGround)
        {
            m_BackGround->removeFromParent();
            m_BackGround->release();
        }
        m_BackGround = executeEffect(SLZ_BATTLEBG, item->BackgroundResID, item->BackgroundCsbAniName);
        CC_SAFE_RETAIN(m_BackGround);
    }

    if (item->BlinkingResID != 0)
    {
        // �������
        if (m_Blink)
        {
            m_Blink->removeFromParent();
            m_Blink->release();
        }
        m_Blink = executeEffect(LZ_FLASH, item->BlinkingResID, item->BlinkingCsbAniName);
        CC_SAFE_RETAIN(m_Blink);
    }

    if (!item->DoodadName.empty())
    {
        // ִ��װ��������л�
        executeDoodad(item, pos);
    }
}

void CUIEffectManager::stopUIEffect()
{
    //�����еĳ����㶯��ֹͣ
    auto node = dynamic_cast<Layer*>(m_Scene->getChildByName("ScaleLayer"));
    if (node)
    {
        node->stopActionByTag(SHAKE_TAG);
    }
}

void CUIEffectManager::executeDark(UIEffectConfItem* conf)
{
    m_DarkNode->setOpacity(0);
    m_DarkNode->stopAllActions();
    m_DarkNode->runAction(Sequence::create(
        DelayTime::create(conf->DarkDelayTime),
        FadeTo::create(conf->DarkFadeInTime, conf->DarkAlpha),
        DelayTime::create(conf->DarkTime),
        FadeOut::create(conf->DarkFadeOutTime),
        RemoveSelf::create(), NULL));

    if (m_DarkNode->getParent() == NULL)
    {
        Layer* scaleLayer = dynamic_cast<Layer*>(m_Scene->getChildByName("ScaleLayer"));
        if (scaleLayer)
        {
            scaleLayer->addChild(m_DarkNode, SLZ_BATTLEBG);
        }
    }
}

void CUIEffectManager::executeShake(int shakeLevel, float shakeDelayTime, float shakeTime)
{
    auto node = dynamic_cast<Layer*>(m_Scene->getChildByName("ScaleLayer"));
    if (node)
    {
        node->stopActionByTag(SHAKE_TAG);
        auto action = Sequence::create(
            DelayTime::create(shakeDelayTime),
            CShake::create(shakeTime, shakeLevel), NULL);
        CHECK_RETURN_VOID(action);
        action->setTag(SHAKE_TAG);
        node->runAction(action);
    }
}

void CUIEffectManager::shakeNode(int tag, int shakeLevel, float shakeDelayTime, float shakeTime)
{
    /*
    auto node = dynamic_cast<Layer*>(m_Scene->getChildByTag(tag));
    CHECK_RETURN_VOID(node);
    node->stopActionByTag(SHAKE_TAG);
    auto action = Sequence::create(
    DelayTime::create(shakeDelayTime),
    CShake::create(shakeTime, shakeLevel), NULL);
    CHECK_RETURN_VOID(action);
    action->setTag(SHAKE_TAG);
    node->runAction(action);*/
}

cocos2d::Node* CUIEffectManager::executeEffect(int zorder, int resId, const std::string& effAni)
{
    // ������Ч
    CEffect* effect = CEffectFactory::createEffect(resId);
    // ���Ŷ�������ӵ�����
    if (effect)
    {
        m_Scene->addChild(effect, zorder);
        effect->playAnimateAutoRemove(effAni);
        effect->setName("ScreenEffect");
        return effect;
    }
    return NULL;
}

void CUIEffectManager::executeDoodad(UIEffectConfItem* conf, const Vec2& pos)
{
    // �����������з�Χ�ڵ�Doodad
    for (auto iter = m_Doodads.begin(); iter != m_Doodads.end();)
    {
        // ��װ��������л�ȡת�������������λ�ã����������ƽڵ�������
        Vec2 worldPos = (*iter)->convertToWorldSpaceAR(Vec2::ZERO);
        if (abs(worldPos.x - pos.x) <= conf->DoodadDistance
            || abs(worldPos.y - pos.y) <= conf->DoodadDistance)
        {
            // ִ�ж���
            auto action = dynamic_cast<ActionTimeline*>((*iter)->getActionByTag((*iter)->getTag()));
            if (action && action->IsAnimationInfoExists(conf->DoodadName))
            {
                if (conf->DoodadStartTime > 0.0f)
                {
                    (*iter)->runAction(Sequence::create(
                        DelayTime::create(conf->DoodadStartTime),
                        CallFunc::create([conf, action](){
                        action->play(conf->DoodadName, conf->DoodadLoop);
                    }), NULL));
                }
                else
                {
                    action->play(conf->DoodadName, conf->DoodadLoop);
                }

                if (action->IsAnimationInfoExists(conf->DoodadBackName))
                {
                    (*iter)->runAction(Sequence::create(
                        DelayTime::create(conf->DoodadStartTime + conf->DoodadContinuedTime),
                        CallFunc::create([conf, action](){
                        action->play(conf->DoodadBackName, conf->DoodadBackLoop);
                    }), NULL));
                }
            }
            // һ���Բ��ŵĶ���
            if (conf->DoodadName == "Break")
            {
                CC_SAFE_RELEASE(*iter);
                iter = m_Doodads.erase(iter);
            }
            else
            {
                iter++;
            }
        }
        else
        {
            iter++;
        }
    }
}

void CUIEffectManager::findDoodads(cocos2d::Node* node)
{
    // ������������Doodad��retain
    for (const auto& child : node->getChildren())
    {
        if (!strcmp(child->getName().c_str(), "Doodad"))
        {
            child->retain();
            m_Doodads.push_back(child);
            // ִ�ж���
            auto action = dynamic_cast<ActionTimeline*>(child->getActionByTag(child->getTag()));
            if (action && action->IsAnimationInfoExists("Normal"))
            {
                action->play("Normal", true);
            }
        }
        findDoodads(child);
    }
}
