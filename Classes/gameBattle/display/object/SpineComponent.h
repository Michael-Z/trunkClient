#ifndef __SPINECOMPONENT_H__
#define __SPINECOMPONENT_H__

#include "AnimateComponent.h"
#include <spine/spine-cocos2dx.h>

class CSpineComponent :
	public CAnimateComponent
{
public:
	CSpineComponent();
	virtual ~CSpineComponent();

	// ���붯���ļ��������������ڵ㣬��onEnter��addChild�����ڵ�
	virtual bool initAnimate(const std::string& fileName, Node* displayNode = NULL);

	// ���붯�������������Ŷ���
	virtual bool playAnimate(const std::string& actionName, int loop = -1);

	// ���ö��������ٶ�
	virtual void setAnimateSpeed(float speed);

	virtual void pause();

	virtual void resume();

    virtual void setAlpha(int alpha)
    {
        if (NULL != m_pSkeletonAnimation)
        {
            m_pSkeletonAnimation->setOpacity(alpha);
        }
    }

    virtual void setColor(cocos2d::Color3B color)
    {
        if (NULL != m_pSkeletonAnimation)
        {
            m_pSkeletonAnimation->setColor(color);
        }
    }

	virtual void onEnter();

	virtual void onExit();

	virtual void setFlipX(bool isFlipX);

	spine::SkeletonAnimation *getSkeletonAnimation()
	{
		return m_pSkeletonAnimation;
	}

    // ������ʱ��Ĭ��
    virtual void setHSV(const VecFloat &hsv);

    // ״̬�л�ʱ����
    virtual void setHue(const float &hue);

private:
    std::string m_FileName;
	spine::SkeletonAnimation *m_pSkeletonAnimation;
};

#endif
