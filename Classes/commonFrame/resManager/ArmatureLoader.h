/*
*   CocoStudio1.6��������������
*   �����������£�
*   1.����armature�̼߳��ز���������
*   2.�����߳̽�������ɵ�������ӵ�Cache
*   3.���ݼ�����ɵ��������Plistͼ������ʱ��ʹ��TextureCache�̼߳�������
*
*   2015-10-30 By ��ү
*/
#ifndef __ARMATURE_LOADER_H__
#define __ARMATURE_LOADER_H__

#include "IResLoader.h"
#include "cocostudio/CocoStudio.h"

enum ArmatureFileType
{
    ArmatureErrorType,
    ArmatureCsbType,
    ArmatureJsonType
};

struct ArmatureLoadingInfo
{
    bool Error;
    int PlistLoadedCount;
    ArmatureFileType FileType;
    std::string ResFile;
    std::string fullPathFile;
    std::list<cocostudio::ArmatureData*> ArmatureDatas;
    std::list<cocostudio::AnimationData*> AnimationDatas;
    std::list<cocostudio::TextureData*> TextureDatas;
    std::map<std::string, std::string> PlistPngMap;
    ResLoadedCallback Callback;
};

class CArmatureLoader : public IResLoader
{
public:
    CArmatureLoader();
    virtual ~CArmatureLoader();
    // Ԥ����һ��������Դ����֧��DragonBone��
    bool addPreloadRes(const std::string& resName, const ResLoadedCallback& callback);
    // ��ʼ�첽����
    bool startLoadResAsyn();
    // �����������
    void onArmatureLoaded(float dt);

    // �Ƿ��Ѽ���ָ������Դ
    virtual bool hasRes(const std::string& resName);
    // �Ƴ�ָ����ArmatureData
    void removeRes(const std::string& resName);
	// ���治������Դ
	void cacheRes(const std::string& resName);
    // �������е�ArmatureData
    void clearRes();

    int getLoadedCount() { return m_nFinishIndex; }
    int getPreloadCount() { return m_LoadingInfos.size(); }
private:
    // ���������߳�
    void armatureThread();
    // ȫ���������
    void onFinish();
private:
    bool m_bIsThreadWorking;                            // ���������̹߳�����ʶ
    int m_nLoadingIndex;                                // �����̼߳�����ɵ������������߳�ִ��++��
    int m_nArmatureIndex;                               // ����������ӵ�ArmatureDataManager��ɵ����������߳�++��
    int m_nFinishIndex;                                 // һ�д�����Ϻ󣨰���addSpriteFrames������������תPNG�̣߳��ص����߳�++��
    std::thread* m_ArmatureThread;                      // ���������߳�
    std::vector<ArmatureLoadingInfo> m_LoadingInfos;    // Ԥ���ع�������
    std::set<std::string> m_ArmatureCache;              // ��Ҫ����Ĺ���
};

class SafeDataReaderHelper : public cocostudio::DataReaderHelper
{
public:
    // �ݲ�֧�֣�����ȥ���tinyxml��
    //static void loadXmlArmatureThreadSafe(ArmatureLoadingInfo& info);
    // ��ȡArmature�б�--���ﲻ�ܷ������ã���Ϊ�����ɾ��
    static std::vector<std::string> getArmatureList();
    static void loadCsbArmatureThreadSafe(ArmatureLoadingInfo& info);
    static void loadJsonArmatureThreadSafe(ArmatureLoadingInfo& info);
    static bool isArmatureLoaded(const std::string& configFile);
    static void saveFileInfo(const std::string& configFile);
};

#endif