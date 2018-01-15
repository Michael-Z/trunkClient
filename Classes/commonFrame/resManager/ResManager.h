/*
*   ��Դ����������
*   1.֧�ָ�����Դ��Ԥ�����Լ�����
*   2.֧�ֲ�����Դ����Ĵ������¡
*   3.֧����Դ������
*
*   2015-10-30 By ��ү
*/
#ifndef __RES_MANAGER_H__
#define __RES_MANAGER_H__

#include "cocos2d.h"
#include "CsbLoader_.h"
#include "ArmatureLoader.h"
#include "SpineLoader.h"

enum LoaderType
{
    LOADER_NONE,
    LOADER_SPINE,
    LOADER_TEXTURE,
    LOADER_CSB,
    LOADER_ARMATURE,
    LOADER_MUSIC,
};

class CResManager : public cocos2d::Ref
{
private:
    CResManager();
    virtual ~CResManager();

public:
    static CResManager* getInstance();
    static void destroy();

    // ���Ԥ���ص���Դ���ڲ����ݺ�׺�Զ�ʶ���ļ���ʽ
    // ֧�����¸�ʽ��csb��2.0����png�����е������ļ�����plist��ͼ����ָ������
    bool addPreloadRes(const std::string& resName, const ResLoadedCallback& callback);
    // ���Ԥ���ص���Դ���ڲ����ݺ�׺�Զ�ʶ���ļ���ʽ
    // ֧�����¸�ʽ��ԣ�json + atlas��plist + png�����е������ļ�����
    bool addPreloadRes(const std::string& resName, const std::string& ass, const ResLoadedCallback& callback);
    // ����cocostudio1.6��������
    bool addPreloadArmature(const std::string& resName, const ResLoadedCallback &callback);
    // ��ʼ������Դ���첽����
    bool startResAsyn();
    // ���ý����ص�
    void setFinishCallback(const ResFinishCallback& callback);

    // ͨ���������ֻ�ù�����������
    spSkeletonData * getSpineSkeletonData(const std::string &resName);
    // ����Spine������������
    spine::SkeletonAnimation* createSpine(const std::string& resName);
    // ��ȡCsb�ڵ����
    cocos2d::Node* getCsbNode(const std::string& resName);
    // ��¡Csb�ڵ����
    cocos2d::Node* cloneCsbNode(const std::string& resName);

    // ɾ��ָ����Դ, armature��csb��׺����, �����ӿ�
    bool removeRes(const std::string& resName);
    bool removeArmature(const std::string& resName);
    // ��һ�ε�clear�����ָ������Դ
    void cacheRes(const std::string& resName);
    void cacheRes(int loaderType);
    // ������Դ
    void clearRes();
    // �Ƿ��������Դ
    bool hasRes(const std::string& resName);
    bool hasArmature(const std::string& resName);

    // �ڼ�����Դ������׷���µ��첽��Դ
    //bool appendLoadResAsyn() { return false; }
private:
    // ��ʼ��
    bool init();
    // ����ļ���׺
    std::string getSuffix(const std::string &filename);
    // ������
    void checkLoading(float dt);

private:
    static CResManager*			m_Instance;
    bool						m_bIsLoading;
    int							m_nPreload;
    int							m_nLoaded;

    ResFinishCallback			m_FinishCallback;
    std::set<int>				m_CacheLoader;
    std::map<int, IResLoader*>	m_Loaders;
};

#endif