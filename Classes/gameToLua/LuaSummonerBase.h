#ifndef _SUM_SUMMONER_BASE_
#define _SUM_SUMMONER_BASE_

#include "LuaTools.h"
#include "BufferData.h"

bool regiestSummonerBase();

void onLuaRespone(CBufferData* buffer);

void onLuaLoginSDKSuccess(int pfType, std::string openId, std::string token);

void onLuaPayQQSuccess(int type, std::string openId, std::string openKey, std::string pf, std::string pfKey);

void onLuaBattleStart(int stageId);

void onLuaBattleOver();

void debugLuaStack();

void onLuaEvent(int eventId);
void onLuaEventWithParamInt(int eventId, int param);
void onLuaEventWithParamStr(int eventId, std::string param);

void onLuaQuitBattle();

void onLuaPlayBgMusic(int musicId);

/* ������� */
void checkPush();
void clearPush();

// ��Ϸ��̨ʱ����draw, ������ȸ���, ��̨Ҳ�����ִ��, �м���ܻᴥ����ӽڵ��¼�
void setLuaBackgroundValue(bool isBackground);

#endif