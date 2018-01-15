#include "SpineEffect.h"
#include "ResPool.h"
USING_NS_CC;

CSpineEffect::CSpineEffect()
: m_pAnimation(NULL)
{
}

CSpineEffect::~CSpineEffect()
{
}

bool CSpineEffect::init(const std::string& fileName)
{
    CHECK_RETURN(CEffect::init());
    m_pAnimation = CResPool::getInstance()->createSpine(fileName);
    CHECK_RETURN(m_pAnimation);
    m_FileName = fileName;
    addChild(m_pAnimation);
    return true;
}

bool CSpineEffect::init(int dir, const EffectConfItem* conf, const std::string& fileName)
{
    CHECK_RETURN(CEffect::init(dir, conf));

    m_pAnimation = CResPool::getInstance()->createSpine(fileName);
    CHECK_RETURN(m_pAnimation);
    m_FileName = fileName;
    addChild(m_pAnimation);

    // ���Ŷ���
    m_pAnimation->setAnimation(0, m_pConf->AnimationName, m_pConf->Loop == 0 ? false : true);

	// ���������ѭ�����е���������������ɺ󵭳�
    spTrackEntry *trackEntry = m_pAnimation->getCurrent();
	if (trackEntry)
	{
		if (m_pConf->Loop == 0
			|| (0 == trackEntry->loop && m_pConf->Loop < 0))
		{
			// ��������
            m_pAnimation->setEndListener([this](int trackIndex)
            {
                runAction(Sequence::create(FadeOut::create(m_pConf->FadeOutTime),
                    RemoveSelf::create(true),
                    NULL));
            });
		}
	}

    if (m_pConf->ZOrderType == EffZOrderGlobal)
    {
        setGlobalZOrder(m_pConf->ZOrder);
    }
    m_pAnimation->setTimeScale(m_pConf->AnimationSpeed);
	return true;
}

void CSpineEffect::onEnter()
{
	CEffect::onEnter();
}

void CSpineEffect::onExit()
{
    if (m_pAnimation != nullptr)
    {
        m_pAnimation->setEndListener(nullptr);
        CResPool::getInstance()->freeSpineAnimation(m_FileName, m_pAnimation);
        removeChild(m_pAnimation);
    }
    CEffect::onExit();
}

// ����ָ������
bool CSpineEffect::playAnimate(const std::string& animate)
{
    CHECK_RETURN(m_pAnimation);
    // ��鶯��
    auto animation = m_pAnimation->setAnimation(0, animate, false);
    return(NULL != animation);
}

// ����ָ�����������ڶ�����������Զ��Ƴ�
bool CSpineEffect::playAnimateAutoRemove(const std::string& animate)
{
    if (playAnimate(animate))
    {
        spTrackEntry *trackEntry = m_pAnimation->getCurrent();
        if (trackEntry)
        {
            // ��������
            m_pAnimation->setEndListener([this](int trackIndex)
            {
                removeFromParent();
            });
        }
        return true;
    }
    return false;
}