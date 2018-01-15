/*
*  �첽���� Csb�ڵ�
*  1. ͨ��addPreloadResAsyn������Ҫ�첽���صĽڵ�ȫ����ӽ������������ø�Csb������ɵĻص���
*  2. ����������startLoadingResAsyn��ʼ�첽���أ����ȵ���setFinishCallback���ü�����ɵĻص���
*  3. �������֮�����ʹ��getCsbNode������ȡ����Ľڵ�
*
*  2015-10-22 By ��ү
*
*/
#ifndef __CSB_LOADER_H__
#define __CSB_LOADER_H__

#include <set>
#include <string>
#include <map>

#include "IResLoader.h"
#include "CommTools.h"

#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/util.h"
#include "cocostudio/FlatBuffersSerialize.h"
#include "cocostudio/CSParseBinary_generated.h"

class CDataObject : public cocos2d::Ref
{
public:
    CDataObject();
    virtual ~CDataObject();

    void setData(cocos2d::Data data);
    cocos2d::Data &getData();

private:
    cocos2d::Data m_Data;
};

struct CsbObject
{
    virtual ~CsbObject();

    std::set<CDataObject*> DataObjects;
    std::set<cocos2d::Texture2D*> Textures;
};

// loading��csb��Ϣ
struct LoadingCsbObject : public cocos2d::Ref
{
    LoadingCsbObject(std::string csbFilePath, ResLoadedCallback callback) 
    : SuccessCount(0)
    , FaileCount(0)
    , CsbFilePath(csbFilePath)
    , MainCsbObject(nullptr)
    , Callback(callback)
    {
    }

    int SuccessCount;
    int FaileCount;
    std::string CsbFilePath;
    CsbObject* MainCsbObject;
    std::set<std::string> TextureFiles;
    ResLoadedCallback Callback;
};

class CNewCsbLoader : public IResLoader
{
public:
    CNewCsbLoader();
    virtual ~CNewCsbLoader();

    // ���һ��Ҫ�첽���ص�Csb�ļ���������ص�����Щ��Ϣ�ᱻ����һ��Ԥ���ض�����
    // �����һ��Ƕ������Csb���ļ�������û������֮ǰȥ������Щ��Csb��autoSearchCsbChild�����Զ�������Ԥ�������ǵ�������Դ
    // ��ӳɹ�����true���Ѿ���ʼ�첽���ػ��Ѿ���Ԥ�����б��з���false
    bool addPreloadRes(const std::string& csbFile, const ResLoadedCallback& callBack);
    // �첽����Ԥ���ض����е���Դ
    bool startLoadResAsyn();
    // �Ƿ��Ѽ���ָ������Դ
    virtual bool hasRes(const std::string& resName);
    // ����ָ����Csb��Դ
	void removeRes(const std::string& csbFile);
	// �������Դ
	void cacheRes(const std::string& resName);
    // �������е�Csb��Դ
    void clearRes();

    void waitForQuit();

    inline int getLoadedCount() { return m_CurrentLoadedCount; }
    inline int getPreloadCount() { return m_TotalLoadingCount; }
    // ����һ��Csb�ڵ�
    cocos2d::Node* createCsbNode(const std::string& csbFile);

private:
    // �߳��м���csb
    void loadCsb();
    // �������Csb����
    void searchTexturesByCsbFile(const cocos2d::Data& data, std::set<std::string>& texSet);
    // ����Csb�ڵ���
    void searchTexturesByCsbNodeTree(const flatbuffers::NodeTree* tree, std::set<std::string>& texSet, std::set<CDataObject*>& objects);
    // ͨ��csb�ļ���ȡdata�����߳����̶߳�����ã���Ҫ����
    CDataObject* getDataForCsbFile(const std::string& csbFile);
    // ����plist�ļ���ȡ��Ӧ�������ļ�����ֻ�����̻߳���ã����̲߳���Ҳ��Ӧ�õ���
    const std::string& getTextFromPlist(const std::string& plist);
    // ֻ�����̻߳���ã�ִ�н����Ļص�
    void onCsbLoadFinish(LoadingCsbObject* loadingCsb, bool success);

    // ����Csb�ڵ㶼������Ϻ�ִ��
    void onLoadFinish();
    void addImageAsyncCallBack(float dt);
    cocos2d::Node* nodeWithFlatBuffers(const flatbuffers::NodeTree *nodetree);
    std::string getGUIClassName(const std::string &name);

private:
    bool m_bNeedQuit;
    int m_CurrentLoadedCount;
    int m_TotalLoadingCount;
    
    std::thread*                           m_LoadingThread;     // �߳��첽����
    std::mutex                             m_RequestMutex;      // ���󻥳���
    std::mutex                             m_ResponseMutex;     // ��Ӧ������
    std::mutex                             m_DataObjectMutex;   // DataObjectPool������
    std::condition_variable                m_SleepCondition;    // ����

    std::deque<LoadingCsbObject*>          m_RequestQueue;      // ���������Դ�Ķ���
    std::deque<LoadingCsbObject*>          m_ResponseQueue;     // ��������Դ�Ķ���
    std::set<LoadingCsbObject*>            m_LoadingTextureCsb; // ���ڼ��������CSBLoading����
    
    std::map<std::string, CDataObject*>    m_DataObjectPool;    // CSB�ļ�dataobject
    std::map<std::string, CsbObject*>      m_CsbObjectPool;     // ������ɵ�Csb�ڵ�����
    std::map<std::string, std::string>     m_PlistTextMap;      // ��
    std::set<std::string>                  m_CacheRes;          // ������Դ�б�
};

#endif
