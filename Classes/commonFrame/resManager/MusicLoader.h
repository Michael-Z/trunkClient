#ifndef __MUSIC_LOADER_H__
#define __MUSIC_LOADER_H__

#include "IResLoader.h"

struct MusicResInfo
{
	bool isLoaded;
	ResLoadedCallback callback;
};

class CMusicLoader : public IResLoader
{
public:
	CMusicLoader();
	~CMusicLoader();

public:
	// ���صĶ���Effect, background����ҪԤ����
	bool addPreloadRes(const std::string& resName, const ResLoadedCallback& callback);
	// ��ʼ����
	bool startLoadResAsyn();
	// �Ƿ��Ѽ���
    bool isMusicLoad(const std::string &resName);
    // �Ƿ��Ѽ���ָ������Դ
    virtual bool hasRes(const std::string& resName) { return isMusicLoad(resName); }
	// �Ƴ���Դ
	void removeRes(const std::string& resName);
	// �ļ�������
	void cacheRes(const std::string& resName);
	// �����Դ
	void clearRes();
	// ����Ѽ��ظ���
	int getLoadedCount();
	// ���Ԥ���ظ���
	int getPreloadCount();

private:

	void loadingMusic(float dt);

private:

	bool								m_bThreadWork;
	int									m_nPreloadCount;
	std::thread* 						m_pThread;
	std::map<std::string, MusicResInfo>	m_loadingEffect;
	std::set<std::string>   m_cacheRes;
	std::set<std::string>	m_loadedEffect;
};

#endif //__MUSIC_LOADER_H__
