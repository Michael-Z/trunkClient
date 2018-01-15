#include "IResLoader.h"

using namespace std;
USING_NS_CC;

IResLoader::IResLoader()
: m_bIsLoading(false)
, m_finishCallback(nullptr)
{
}


IResLoader::~IResLoader()
{
}

bool IResLoader::startLoadResAsyn()
{
    return !m_bIsLoading;
}

void IResLoader::clearRes()
{
    m_AutoLoadResCache.clear();
}

void IResLoader::loadResAsyn(const std::string& resName, const ResLoadedCallback& callback)
{
}

bool IResLoader::addPreloadRes(const std::string& resName, const ResLoadedCallback& callback)
{
    return false;
}

bool IResLoader::addPreloadRes(const std::string& resName, const std::string& assName, const ResLoadedCallback& callback)
{
    return false;
}

bool IResLoader::checkLoadRes(const std::string& resName, const std::string& assName, const ResLoadedCallback& callback)
{
    string fullPath = FileUtils::getInstance()->fullPathForFilename(resName);
    // 1.���·���Ƿ���ȷ
    if (fullPath.empty())
    {
        CCLOG("IResLoader::addPreloadRes can't find file %s ", resName.c_str());
        return false;
    }

    // 2.����Ƿ��Ѿ�������
    if (hasRes(fullPath))
    {
        if (callback != nullptr)
        {
            callback(fullPath, true);
        }
        return false;
    }

    // 3.����Ƿ����ڼ�����
    if (m_bIsLoading)
    {
        PreLoadResInfo info;
        info.ResName = resName;
        info.AssName = assName;
        info.Callback = callback;
        m_AutoLoadResCache.push_back(info);
        return false;
    }

    // ���Լ��ظ���Դ
    return true;
}

void IResLoader::autoLoadRes()
{
    // û����Դ��Ҫ�Զ�����
    if (m_AutoLoadResCache.size() == 0)
    {
        return;
    }

    // �ȸ���һ�ݣ���Ϊ���ڱ��������ɾ��
    vector<PreLoadResInfo> AutoLoadResCache = m_AutoLoadResCache;
    m_AutoLoadResCache.clear();

    for (auto& resInfo : AutoLoadResCache)
    {
        // �Ƿ��Ѽ���
        if (hasRes(resInfo.ResName))
        {
            resInfo.Callback(resInfo.ResName, true);
            continue;
        }

        if (resInfo.AssName.empty())
        {
            addPreloadRes(resInfo.ResName, resInfo.Callback);
        }
        else
        {
            addPreloadRes(resInfo.ResName, resInfo.AssName, resInfo.Callback);
        }
    }

    // �����Ҫ���صģ��Զ���ʼ����
    startLoadResAsyn();
}

std::string TextureTools::getTexturePathFromPlist(std::string plistPath)
{
    string texturePath("");

    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(plistPath);
    if (fullPath.size() == 0)
    {
        return texturePath;
    }

    ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath);

    if (dict.find("metadata") != dict.end())
    {
        ValueMap& metadataDict = dict["metadata"].asValueMap();
        // try to read  texture file name from meta data
        texturePath = metadataDict["textureFileName"].asString();
    }

    if (!texturePath.empty())
    {
        // build texture path relative to plist file
        texturePath = FileUtils::getInstance()->fullPathFromRelativeFile(texturePath, plistPath);
    }
    else
    {
        // build texture path by replacing file extension
        texturePath = plistPath;

        // remove .xxx
        size_t startPos = texturePath.find_last_of(".");
        texturePath = texturePath.erase(startPos);

        // append .png
        texturePath = texturePath.append(".png");
    }
    return texturePath;
}
