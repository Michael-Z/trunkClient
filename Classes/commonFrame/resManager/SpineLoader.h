/*
*   �첽����Spine����
*   �����Ԥ���ص���Դ��ִ���첽���أ���ʹ����2���߳�
*   1.ʹ��TextureCache���߳��첽��������
*   2.���ؽ�������ݼ�����ɵ�������spAtlas����
*   3.������������̣߳�ʹ��spAtlas���󴴽��������ݶ���
*   4.�����߳��д�����ɵĹ�����Cache��
*
*   2015-10-30 by ��ү
*/
#ifndef __SPINE_LAODER_H__
#define __SPINE_LAODER_H__

#include <spine/spine-cocos2dx.h>
#include <atomic> 
#include "IResLoader.h"

struct SpineLoadingInfo
{
    std::string JsonFile;
    std::string AtlasFile;
    std::string TextureFile;
    cocos2d::Image* AtlasImage;
    spAtlas* Atlas;
    spSkeletonData* SkeletonData;
    ResLoadedCallback Callback;
};

struct SpineCacheInfo
{
    spAtlas* Atlas;
    spSkeletonData* SkeletonData;
};

class CSpineLoader : public IResLoader
{
public:
    CSpineLoader();
    virtual ~CSpineLoader();

    // Ԥ������Դ����û�п�ʼ����
    virtual bool addPreloadRes(const std::string& resName, const std::string& atlasName, const ResLoadedCallback& callback);
    // Ԥ������Դ����û�п�ʼ���أ��贫�븱��Դ��Spine��json + atlas��
    virtual bool startLoadResAsyn();

    void onSkeletonLoaded(float dt);

    // ��ȡһ����Դ
    spSkeletonData* getSkeletonData(const std::string& resName);
    // �Ƿ��Ѽ���ָ������Դ
    virtual bool hasRes(const std::string& resName) { return getSkeletonData(resName) != nullptr; }
    // �Ƴ�һ����Դ
    virtual void removeRes(const std::string& resName);
	// ������Դ
	virtual void cacheRes(const std::string& resName);
    // ���������Դ
    virtual void clearRes();
    // �Ѽ�����ɵ���Դ����
    virtual int getLoadedCount(){ return m_nFinishIndex; }
    // ��ȡԤ���ص���Դ����
    virtual int getPreloadCount(){ return m_LoadingInfos.size(); }

private:
    // ���ؽ���ʱ����
    void onFinish();
    // ���������߳�
    //void skeletonThread();
    // ������һ������
    //void loadNextSkeletonThreadSafe();
    // ������һ������
    //void loadNextTextureThreadSafe();

private:
    bool m_bThreadWorking;
    int m_nTextureLoadingIndex;
    int m_nSkeletonLoadingIndex;                       // ��ǰ�ɼ���
    int m_nFinishIndex;
    std::thread* m_SkeletonThread;
    std::vector<SpineLoadingInfo> m_LoadingInfos;
    std::set<std::string> m_LoadingSpine;
	std::set<std::string> m_CacheRes;
    std::map<std::string, SpineCacheInfo> m_SpineCache;
};

#endif
