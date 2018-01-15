/*
* ��Ϸ�е���Ч������
*
* 1.������ЧID������Ч����
* 2.��ӵ������п��Լ������Լ�
* 3.���Ҫ����λ�ã�����addChild����֮ǰ����
*
* 2015-1-28 by ��ү
*/
#ifndef __EFFECT_FACTORY_H__
#define __EFFECT_FACTORY_H__

#include "cocos2d.h"
#include "KxCSComm.h"
#include "Effect.h"

class CEffectFactory
{
public:
    CEffectFactory();
    virtual ~CEffectFactory();

    // ���������Ч��parent�ڵ�
    static bool createEffectsToNode(const VecInt& effIds,
        cocos2d::Node* parent, int dir = 1, int zorder = 0, float delay = 0.0f, cocos2d::Vec2 pos = Vec2::ZERO);
    // ����һ����Ч
    static CEffect* create(int effId, int dir = 1, int zorder = 0, float delay = 0.0f);
	static CEffect* createEffect(int resID);
};

#endif
