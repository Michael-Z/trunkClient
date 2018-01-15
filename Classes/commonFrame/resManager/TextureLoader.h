#ifndef __TEXTURE_LOADER_H__
#define __TEXTURE_LOADER_H__

#include "IResLoader.h"
#include <map>

struct TextureLoadInfo
{
	bool				isLoaded;		//�Ƿ��Ѽ������
	ResLoadedCallback	callback;		//�ص�����
};

class CTextureLoader : public IResLoader
{
public:
	CTextureLoader();
	~CTextureLoader();

public:
	// Ԥ������Դ����û�п�ʼ����
	bool addPreloadRes(const std::string& resName, const ResLoadedCallback& callback);
	// ��ʼ�첽������Դ
    bool startLoadResAsyn();

    // �Ƿ��Ѽ���ָ������Դ
    virtual bool hasRes(const std::string& resName) { return m_TextureCache.find(resName) != m_TextureCache.end(); }
	// �Ƴ�һ����Դ
	void removeRes(const std::string& resName);
	// ������Դ
	void cacheRes(const std::string& resName);
	// ���������Դ
	void clearRes();
	// �Ѽ�����ɵ���Դ����
	int getLoadedCount();
	// ��ȡԤ���ص���Դ����
	int getPreloadCount();

private:
	// ����ļ���׺
	std::string getSuffix(const std::string &filename);

private:

	int										    m_nPreloadCount;
    int                                         m_nLoadedCount;
    std::map<std::string, cocos2d::Texture2D*>  m_TextureCache;
    std::map<std::string, TextureLoadInfo>      m_preloadList;
	std::set<std::string>					    m_cacheRes;
};

#endif //__TEXTURE_LOADER_H__
