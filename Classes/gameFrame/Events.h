/*
* �����¼���Ϣͷ�ļ�
* ��������EventManagerת������ϢID
* ������ص���Ϣ����Э����ϢID����
*
* 2015-2-7 By ��ү
*/
#ifndef __EVENTS_H__
#define __EVENTS_H__

enum EventsType
{
    LoginEvents = 0,                // ��¼�����е��¼�

    MainEvents = 100,               // �������е��¼�

    BattleEvents = 200,             // ս�������е��¼�
    BattleEventTouchReleaseSkill,   // ������ͷż���
    BattleEventTouchCancelSkill,    // ȡ���ͷż���
	BattleEventTouchPlaySkill,      // �ɹ��ͷż���
    BattleEventShowTips,            // �����ʾ��Ϣ
	BattleEventEnemyActionTips,		// �Է��ɱ�,�ͷż�����ʾ
    BattleEventTouchReleaseTips,    // �ͷż�����ʾ
    BattleEventTouchCancelTips,     // ȡ��������ʾ
    BattleEventFightStartTips,      // ս����ʼ��ʾ
    BattleEventCrystalUpgrade,      // ˮ�������¼�

    BattleEventTalkCommand,         // pvp ��Ϸ

    BattleEventDispatchHero,        // ��ǲӢ��

};

#endif