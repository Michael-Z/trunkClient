/************************************************************************
FMOD���ֲ�������                                                         
ע�⣺1��fmod�Ŀ��L��Ϊdebugģʽʹ�ã�������release
     2��fmod�Ŀ��32λ��64λ
     3����Ҫ����ѭ���е���update����

2016-7-29                                                        
************************************************************************/

#ifndef __FMOD_AUDIO_ENGINE_H__
#define __FMOD_AUDIO_ENGINE_H__

#include "fmod/include/fmod_studio.hpp"
#include "cocos2d.h"

class CFMODAudioEngine
{
private:
    CFMODAudioEngine();
    ~CFMODAudioEngine();

public:
    static CFMODAudioEngine *getInstance();
    static void destory();

    static const int INVALID_AUDIO_ID;

    /** ����bank�ļ�
    * @param bank�ļ�
    * @return trueΪ���سɹ�,falseΪʧ��
    */
    bool loadBankFile(const char *bankFile);
    
    /** ж��bank�ļ�
    * @param bank�ļ�
    * @return trueΪж�سɹ�,falseΪʧ��
    */
    bool unloadBankFile(const char *bankFile);

    /** ÿһ֡����
    */
    void update();

    /** ���ű�������
    * @param ��������·��,��event:/Music/city,FMOD Studio�в鿴
    */
    void playBackgroundMusic(const char *musicPath);

    /** ֹͣ��������
    */
    void stopBackgroundMusic();
    
    /** ���ñ������ֲ���
    * @param ��������,��level,fmod studio�в鿴
    * @param ֵ
    */
    void setBackgroundMusicParam(const char *param, float value);

    /** ������Ч
    * @param ��Ч·��,��event:/UI/Click0,FMOD Studio�в鿴
    * @param ����ֵ
    */
    int playEffect(const char *eventPath, float volume = 1.0);

    /** ���ݲ������ò�����Ч
    * @param ��Ч·��,��event:/UI/Click0,FMOD Studio�в鿴
    * @param ��������
    * @param ֵ
    */
    int playEffectWithParam(const char *eventPath, const char *param, float value);

    /** ��ȡ����
    * @return ������С0-1
    */
    float getMusicVolume();

    /** ��������
    * @param ֵ0-1
    */
    void setMusicVolume(float volume);

    /** ���ñ��������Ƿ���ͣ
    * @param ����ֵ,trueΪ��ͣ��false�ָ�
    */
    void setPaused(bool isPause);

    /* ���������Ƿ��ڲ���
    * @return ������true������false
    */
    bool isBackgroundMusicPlaying();

    /** �ָ���Ч
    * @param ����playEffect���ص���ЧID
    */
    void resumeEffect(unsigned int soundId);

    /** ��ͣ��Ч
    * @param ����playEffect���ص���ЧID
    */
    void pauseEffect(unsigned int soundId);

    /** ֹͣ��Ч
    * @param ����playEffect���ص���ЧID
    */
    void stopEffect(unsigned int soundId);

    /** ���������Ч
    */
    void clearAllEffects();

    /* �����Ƿ�ر���Ч
    */
    void setOpenEffect(bool isOpen);

    /* ��ͣ�߳�
    */
    void mixerSuspend();

    /* �ָ��߳�
    */
    void mixerResume();

protected:

    /** ��ȡ�����¼�����
    * @param ����·��,��event:/Music/city,fmod studio�в鿴
    * @return EventInstance����
    */
    FMOD::Studio::EventInstance *getEventInstance(const char *eventPath);

    /** ��ȡϵͳ
    * @return System���� Ψһ
    */
    FMOD::Studio::System *getStudioSystem();

    /** �ͷ�ϵͳ
    */
    void endStudioSystem();

    /* ��ȡ��Ч����,ȥ��·��
    * @return ��Ч����
    */
    std::string getFileName(std::string path);

protected:
    static CFMODAudioEngine *m_pInstance;

    bool m_bIsOpenEffect;                                     // �Ƿ�����Ч
    int m_nCurrentAudioID;                                    // ��ǰ��ЧID,����
    std::map<int, FMOD::Studio::EventInstance*> m_mapEffects; // ��Ч�б�<soundId, ����>
    std::map<std::string, int> m_mapEffectNames;              // ��Ч��Դ�б�<��Ч��, soundId>
};

#endif //__FMOD_AUDIO_ENGINE_H__
