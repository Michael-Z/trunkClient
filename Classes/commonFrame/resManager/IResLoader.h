/*
*   ��Դ�������Ļ���
*   �����˸���������Դ��������ͨ�ýӿ�
*   ������ͨ�õ���Դ���ػص�
*
*   2015-10-27 By ��ү
*/
#ifndef __IRES_LOADER_H__
#define __IRES_LOADER_H__

#include <cocos2d.h>
#include <stdio.h>
#include <string>

// ������Դ������ɵĻص�
// ������ص���Դ���Լ����ؽ��
typedef std::function<void(const std::string&, bool)> ResLoadedCallback;
// ������Դ������ɺ�Ļص�
// ���δ�������Դ���ɹ���Դ������
typedef std::function<void(int, int)> ResFinishCallback;

// Ԥ���ؽṹ��
struct PreLoadResInfo
{
    std::string ResName;
    std::string AssName;
    ResLoadedCallback Callback;
};

class IResLoader : public cocos2d::Ref
{
public:
    IResLoader();
    virtual ~IResLoader();

    // Ԥ������Դ����û�п�ʼ����
    virtual bool addPreloadRes(const std::string& resName, const ResLoadedCallback& callback);
    // Ԥ������Դ����û�п�ʼ���أ��贫�븱��Դ��Spine��json + atlas��
    virtual bool addPreloadRes(const std::string& resName, const std::string& assName, const ResLoadedCallback& callback);

    // ��ʼ�첽������Դ
    virtual bool startLoadResAsyn();
    // �Ƿ��Ѽ���ָ������Դ
    virtual bool hasRes(const std::string& resName) = 0;
    // �Ƴ�һ����Դ
    virtual void removeRes(const std::string& resName) = 0;
	// �ļ�������
	virtual void cacheRes(const std::string& resName) = 0;
    // ���������Դ
    virtual void clearRes();
    // �Ѽ�����ɵ���Դ����
    virtual int getLoadedCount() = 0;
    // ��ȡԤ���ص���Դ����
    virtual int getPreloadCount() = 0;

    // ��ѡʵ�ֽӿڣ�ֱ���첽����ĳ��Դ����Ԥ����
    // ���Լ���֤�̰߳�ȫ��Ԥ����ʱ���������
    virtual void loadResAsyn(const std::string& resName, const ResLoadedCallback& callback);

    // ����������Դ������ɵĻص�
    virtual void setFinishCallback(const ResFinishCallback& callback)
    {
        m_finishCallback = callback;
    }
    // �Ƿ����ڼ���
    virtual bool isLoading() { return m_bIsLoading; }

protected:
    // ����Ƿ���Լ���ָ������Դ
    virtual bool checkLoadRes(const std::string& resName, const std::string& assName, const ResLoadedCallback& callback);
    // �Զ����� ���� �ڡ������С�Ҫ���ص���Դ�ᱻ��ӵ��Զ������б���
    // ����Զ������б��е���Դ�Ѿ������أ������ûص��������������Զ������б����Ƴ���
    // �����Դδ�����أ�����ظ���Դ���������Զ������б����Ƴ���
    // �����Դ�Ѿ�׼�����أ��򡾱������Զ������б��С�
    virtual void autoLoadRes();

protected:
    bool m_bIsLoading;
    ResFinishCallback m_finishCallback;
    std::vector<PreLoadResInfo> m_AutoLoadResCache;     // �Զ������б�
};

namespace TextureTools
{
    std::string getTexturePathFromPlist(std::string plistPath);
}

#endif
