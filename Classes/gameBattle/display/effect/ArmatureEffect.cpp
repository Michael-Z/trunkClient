#include "ArmatureEffect.h"
#include "ConfOther.h"

USING_NS_CC;
using namespace cocostudio;

// ��Armature���������е����ӵ���Ϊ����˶�ģʽ
void updateParticle(Armature* node)
{
	for (auto& boneItem : node->getBoneDic())
	{
		auto& bone = boneItem.second;
		if (bone->getDisplayRenderNodeType() == CS_DISPLAY_PARTICLE)
		{
			auto particle = dynamic_cast<ParticleSystem*>(bone->getDisplayRenderNode());
			if (nullptr != particle)
			{
				particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
			}
		}
	}
}

// ����Armature���������ȫ��ZOrder
void updateArmatureGrobleZOrder(Armature* node, float zorder)
{
    for (auto& n : node->getBoneDic())
    {
        // �������й�����DisplayManager
        Bone* bone = n.second;
        if (nullptr != bone && nullptr != bone->getDisplayManager())
        {
            // ��������DisplayManager��DecorativeDisplayList
            auto& l = bone->getDisplayManager()->getDecorativeDisplayList();
            for (auto ln : l)
            {
                // ����������GlobalZOrder
                ln->getDisplay()->setGlobalZOrder(zorder);
            }
        }
    }
}

CArmatureEffect::CArmatureEffect()
: m_pAnimation(NULL)
{
}

CArmatureEffect::~CArmatureEffect()
{
}

bool CArmatureEffect::init(const std::string& armature)
{
    CHECK_RETURN(CEffect::init());
	m_pAnimation = Armature::create(armature);
    CHECK_RETURN(m_pAnimation);
    addChild(m_pAnimation);
    return true;
}

bool CArmatureEffect::init(int dir, const EffectConfItem* conf)
{
    CHECK_RETURN(CEffect::init(dir, conf));

	auto confResPaht = queryConfSResInfo(m_pConf->ResID);
	CHECK_RETURN(confResPaht);
	m_pAnimation = Armature::create(confResPaht->ResName);
    CHECK_RETURN(m_pAnimation);
    addChild(m_pAnimation);

	// ��鶯��
    auto animation = m_pAnimation->getAnimation()->getAnimationData()->getMovement(m_pConf->AnimationName);
	CHECK_RETURN(NULL != animation);

	// ���������ѭ�����е���������������ɺ󵭳�
	if (m_pConf->Loop == 0
		|| (!animation->loop && m_pConf->Loop < 0))
	{
        m_pAnimation->getAnimation()->setMovementEventCallFunc(
			[this](Armature* armature, MovementEventType movementType, const std::string movementId)->void
		{
			if (movementType == COMPLETE)
			{
				runAction(Sequence::create(FadeOut::create(m_pConf->FadeOutTime),
					RemoveSelf::create(true),
					NULL));
			}
		});
	}

    m_pAnimation->getAnimation()->setSpeedScale(m_pConf->AnimationSpeed);

	// �ݹ��������
    updateParticle(m_pAnimation);
	return true;
}

void CArmatureEffect::onEnter()
{
	CEffect::onEnter();
    if (m_pConf)
    {
        // ���Ŷ���
        m_pAnimation->getAnimation()->play(m_pConf->AnimationName, -1, m_pConf->Loop);
    }
}

// ����ָ������
bool CArmatureEffect::playAnimate(const std::string& animate)
{
    CHECK_RETURN(m_pAnimation);
    // ��鶯��
    auto animation = m_pAnimation->getAnimation()->getAnimationData()->getMovement(animate);
    CHECK_RETURN(NULL != animation);
    m_pAnimation->getAnimation()->play(animate);
    return true;
}

// ����ָ�����������ڶ�����������Զ��Ƴ�
bool CArmatureEffect::playAnimateAutoRemove(const std::string& animate)
{
    if (playAnimate(animate))
    {
        m_pAnimation->getAnimation()->setMovementEventCallFunc(
            [this](Armature* armature, MovementEventType movementType, const std::string movementId)->void
        {
            if (movementType == COMPLETE)
            {
                removeFromParent();
            }
        });
        return true;
    }
    return false;
}