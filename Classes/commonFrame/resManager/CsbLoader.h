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

#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/util.h"
#include "cocostudio/FlatBuffersSerialize.h"
#include "cocostudio/CSParseBinary_generated.h"

/*struct CsbLoadInfo
{
    std::string CsbFilePath;
    std::map<std::string, Data> CsbFileData;
    std::map<std::string, cocos2d::Image*> PngFiles; 
};*/

#define TAG_CSB_ACTION  1000000

struct CsbLoadingInfo
{
    int LoadedCount;
    std::string CsbFilePath;
    std::set<std::string> PreloadTextures;
    ResLoadedCallback Callback;
};

class CCsbLoader : public IResLoader
{
public:
    CCsbLoader();
    virtual ~CCsbLoader();

    // ���һ��Ҫ�첽���ص�Csb�ļ���������ص�����Щ��Ϣ�ᱻ����һ��Ԥ���ض�����
    // �����һ��Ƕ������Csb���ļ�������û������֮ǰȥ������Щ��Csb��autoSearchCsbChild�����Զ�������Ԥ�������ǵ�������Դ
    // ��ӳɹ�����true���Ѿ���ʼ�첽���ػ��Ѿ���Ԥ�����б��з���false
    bool addPreloadRes(const std::string& csbFile, const ResLoadedCallback& callBack);
    // �첽����Ԥ���ض����е���Դ
    bool startLoadResAsyn();
    // �Ƿ��Ѽ���ָ������Դ
    virtual bool hasRes(const std::string& resName) { return getCsbNode(resName) != nullptr; }
    // ����ָ����Csb��Դ
	void removeRes(const std::string& csbFile);
	// �������Դ
	void cacheRes(const std::string& resName);
    // �������е�Csb��Դ
    void clearRes();

    // ��ȡCache��Csb�ڵ�
    cocos2d::Node* getCsbNode(const std::string& csbFile);
    // update�����������߳��и����Ѽ��ص�������Csb
    void createCsbNodeByLoadedTexture(float dt);

    inline int getLoadedCount() { return m_CurrentLoadedCount; }
    inline int getPreloadCount() { return m_LoadingQueue.size(); }

private:
    // ����һ��Csb�ڵ�
    void createCsbNode(const CsbLoadingInfo& csbInfo);
    cocos2d::Node* nodeWithFlatBuffers(const flatbuffers::NodeTree *nodetree);
    // �������Csb����
    void searchTexturesByCsbFile(cocos2d::Data& data, std::set<std::string>& texSet);
    // ����Csb�ڵ���
    void searchTexturesByCsbNodeTree(const flatbuffers::NodeTree* tree, std::set<std::string>& texSet);
    // ����Csb�ڵ㶼������Ϻ�ִ��
    void onLoadFinish();
    // �߳��첽���ط���
    // void loadCsbNodeThreadSafe(const std::string& csbFile, CsbLoadInfo& info);

private:
    bool m_bIsAutoSerachSubCsb;
    int m_CurrentLoadedCount;

    // �߳��첽����
    //std::thread* m_LoadingThread;
    //std::set<std::string> m_TextureCacheMirror;       // PNG����ʹ�̼߳����в�����TextureCache�����ж��Ƿ��Ѿ����ع���PNG
    //std::set<std::string> m_CsbNodeMirror;            // CSB�ļ�����ʹ�̼߳����в�����m_CsbNodes�����ж��Ƿ��Ѿ����ع���CSB
    //std::vector<CsbLoadInfo> m_LoadingCsbInfo;

    std::vector<CsbLoadingInfo> m_LoadingQueue;         // �첽���ض���
    std::set<std::string> m_LoadingTextures;            // �������ص�������ֹ�ظ�����
    std::set<std::string> m_LoadingPlists;              // �������ص�ͼ������ֹ�ظ�����
    std::set<std::string> m_CheckedCsb;                 // �Ѿ���������Csb����ֹ�ظ�����
	std::set<std::string> m_CacheRes;					// ������Դ�б�
    std::map<std::string, cocos2d::Data> m_CsbFileCache;// CSB�ļ�����
    std::map<std::string, cocos2d::Node*> m_CsbNodes;   // ������ɵ�Csb�ڵ�����
};

#endif
