/*
 * ������ʾ�������
 * 1.�ṩ����������صı�׼�ӿ�
 * 2.�ṩ�����ص�
 * 3.���ö��������ٶ�
 * 4.��������
 *
 * 2014-12-23 by ��ү
 */
#ifndef __ANIMATE_COMPONENT_H__
#define __ANIMATE_COMPONENT_H__

#include "DisplayComponent.h"

class CAnimateComponent : public CDisplayComponent
{
public:
    CAnimateComponent();
    ~CAnimateComponent();

    // ���붯���ļ��������������ڵ㣬��onEnter��addChild�����ڵ�
	virtual bool initAnimate(const std::string& fileName, Node* displayNode = NULL) { return false; }

    // ���붯��ID�����Ŷ���
    virtual bool playAnimate(int actionId, int loop = -1) { return false; }

    // ���붯�������������Ŷ���
    virtual bool playAnimate(const std::string& actionName, int loop = -1) { return false; }
    
    virtual void setCascadeColorAndOpacityEnabled(cocos2d::Node* node);

    // ���ö��������ٶ�
    virtual void setAnimateSpeed(float speed) {}

    virtual void pause() {}

	virtual void resume() {}

    virtual void setAlpha(int alpha) {}
    virtual void setColor(cocos2d::Color3B color) {}

    virtual void setHSV(const VecFloat &hsv){}

    virtual void setHue(const float &hue){}

	virtual void setFlipX(bool isFlipX) {}

    inline bool isMutex() { return m_bIsMutex; }

    virtual void setMutex(bool ismutex) 
    {
        m_bIsMutex = ismutex; 
        if (m_bIsMutex)
        {
            _name = "MainAnimate";
        }
        else
        {
            _name = "SubAnimate";
        }
    }

	inline void setDisplayNode(Node* displayNode)
	{
		if (NULL != displayNode)
		{
			m_DisplayNode = displayNode;
		}
	}

	inline Node* getDisplayNode()
	{
		if (NULL != m_DisplayNode)
		{
			return m_DisplayNode;
		}
		else
		{
			return _owner;
		}
	}

    inline Node* getMainAnimate()
    {
        return m_MainAnimate;
    }
protected:
    bool m_bIsMutex;        // �ö����Ƿ񻥳�
    bool m_bIsFlipX;

	Node* m_DisplayNode;    // ������role�ϵ���Ҫ�ټ�һ���ڵ�, ����Ŵ���СӰ����������
    Node* m_MainAnimate;    // ������

    cocos2d::GLProgramState *m_pDefaultProgram;      // Ĭ��shader
    cocos2d::GLProgramState *m_pStatusProgram;       // ״̬shader(�ж���������Ч��)
};

#endif
