/*
* �ӵ���ʾ���
*
* 2014-12-23 by ��ү
*/
#ifndef __BULLET_DISPLAY_COMPONENT_H__
#define __BULLET_DISPLAY_COMPONENT_H__

#include "DisplayComponent.h"

class CGameObject;
class CBullet;
class BulletConfItem;
class CArmatureComponent;
class CBulletComponent;
class CBulletDisplayComponent : public CDisplayComponent
{
public:
    CBulletDisplayComponent();
    virtual ~CBulletDisplayComponent();

public:
    //��ʼ��
    bool init(CBullet *bullet, CBulletComponent *logicCom);
    //
    void update(float dt);
    //�Ƿ���ʾ����
    bool isDisplayOver();
    //��������, ���м����н���
    void displayAnimation(int state);

private:
    bool                    m_bIsOver;
    bool                    m_bIsFirstRotate;
    int                     m_nState;
    Vec2                    m_Direction;
    Vec2                    m_Offset;
    Vec2                    m_CurTargetPos;
    CBullet*                m_pOwner;
    CBulletComponent*       m_pBulletComponent;
    const BulletConfItem*   m_pBulletConf;
};

#endif
