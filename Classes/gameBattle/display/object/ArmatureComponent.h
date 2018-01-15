/*
* ����������ʾ���
*
* 2014-12-23 by ��ү
*/
#ifndef __ARMATURE_COMPONENT_H__
#define __ARMATURE_COMPONENT_H__

#include "AnimateComponent.h"
#include "cocostudio/CocoStudio.h"

class CArmatureComponent : public CAnimateComponent
{
public:
    CArmatureComponent();
    virtual ~CArmatureComponent();

    // ���붯���ļ��������������ڵ㣬��onEnter��addChild�����ڵ�
	virtual bool initAnimate(const std::string& fileName, Node* displayNode = NULL);

    // ���붯��ID�����Ŷ���
    // loop Ϊ-1��ʾʹ�ö����ļ��ڲ����ݲ��ţ�0��ʾ����һ�Σ�����0��ʾѭ��N��
    virtual bool playAnimate(const std::string& actionName, int loop = -1);

    // ���ö��������ٶ�
    virtual void setAnimateSpeed(float speed);

    virtual void onEnter();

    virtual void onExit();

    virtual void pause();

    virtual void resume();

    virtual void setAlpha(int alpha)
    {
        if (NULL != m_Armature)
        {
            m_Armature->setOpacity(alpha);
        }
    }

    virtual void setColor(cocos2d::Color3B color)
    {
        if (NULL != m_Armature)
        {
            m_Armature->setColor(color);
        }
    }

    virtual void setMainDisplay(bool isMain)
    {
        if (isMain)
        {
            m_Armature->setTag(MAIN_DISPLAY);
        }
        else
        {
            m_Armature->setTag(-1);
        }
    }

	virtual void setFlipX(bool isFlipX)
	{
        if (m_bIsFlipX != isFlipX)
        {
            m_bIsFlipX = isFlipX;
            // 2dx ������д��
            m_Armature->setScaleX(isFlipX ? -1 : 1 * std::abs(m_Armature->getScaleX()));
        }
	}

    cocostudio::Armature* getArmature()
    {
        return m_Armature;
    }

private:
    cocostudio::Armature* m_Armature;
};

#endif
