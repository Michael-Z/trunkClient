#include "CsbLoader_.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "CsbTool.h"

#include "base/ObjectFactory.h"

#include "../../cocos/ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/CSParseBinary_generated.h"

#include "cocostudio/WidgetReader/NodeReaderProtocol.h"
#include "cocostudio/WidgetReader/NodeReaderDefine.h"

#include "cocostudio/WidgetReader/NodeReader/NodeReader.h"
#include "cocostudio/WidgetReader/SingleNodeReader/SingleNodeReader.h"
#include "cocostudio/WidgetReader/SpriteReader/SpriteReader.h"
#include "cocostudio/WidgetReader/ParticleReader/ParticleReader.h"
#include "cocostudio/WidgetReader/GameMapReader/GameMapReader.h"
#include "cocostudio/WidgetReader/ProjectNodeReader/ProjectNodeReader.h"
#include "cocostudio/WidgetReader/ComAudioReader/ComAudioReader.h"

USING_NS_CC;
using namespace std;
using namespace flatbuffers;
using namespace cocostudio;
using namespace ui;

CDataObject::CDataObject()
{
}

CDataObject::~CDataObject()
{
}

void CDataObject::setData(cocos2d::Data data)
{
    m_Data = data;
}

Data& CDataObject::getData()
{
    return m_Data;
}

CsbObject::~CsbObject()
{
    for (auto iter : DataObjects)
    {
        iter->release();
    }
    DataObjects.clear();
    for (auto iter : Textures)
    {
        iter->release();
    }
    Textures.clear();
}

CNewCsbLoader::CNewCsbLoader()
: m_CurrentLoadedCount(0)
, m_TotalLoadingCount(0)
, m_bNeedQuit(false)
, m_LoadingThread(nullptr)
{
}

CNewCsbLoader::~CNewCsbLoader()
{
    waitForQuit();
    releaseAndClearMap(m_DataObjectPool);
    deleteAndClearMap(m_CsbObjectPool);
}

bool CNewCsbLoader::addPreloadRes(const std::string& csbFile, const ResLoadedCallback& callBack)
{
	//CCLOG("add csb res %s", csbFile.c_str());
    if (!checkLoadRes(csbFile, "", callBack))
    {
        return false;
    }

    string fullPath = FileUtils::getInstance()->fullPathForFilename(csbFile);
    LoadingCsbObject *pLoadingCsb = new LoadingCsbObject(fullPath, callBack);
    m_RequestMutex.lock();
    m_RequestQueue.push_back(pLoadingCsb);
    m_RequestMutex.unlock();
    ++m_TotalLoadingCount;
    LOGDEBUG("performance: CNewCsbLoader Load %s", csbFile.c_str());
    return true;
}

bool CNewCsbLoader::startLoadResAsyn()
{
    if (m_TotalLoadingCount <= 0 || !IResLoader::startLoadResAsyn())
    {
        return false;
    }

    // ��ʼ��CSLoader
    CSLoader::getInstance();

    m_bIsLoading = true;
    m_bNeedQuit = false;
    m_CurrentLoadedCount = 0;

    // �����߳�
    if (nullptr == m_LoadingThread)
    {
        m_LoadingThread = new std::thread(&CNewCsbLoader::loadCsb, this);
    }

    // ��CSB�����Ϻ������߳��м���ͼƬ
    Director::getInstance()->getScheduler()->schedule(
        CC_SCHEDULE_SELECTOR(CNewCsbLoader::addImageAsyncCallBack), this, 0, false);

    return true;
}

bool CNewCsbLoader::hasRes(const std::string& resName)
{
    return m_CsbObjectPool.find(resName) != m_CsbObjectPool.end();
}

void CNewCsbLoader::removeRes(const std::string& csbFile)
{
    string fullPath = FileUtils::getInstance()->fullPathForFilename(csbFile);
    auto iter = m_CsbObjectPool.find(fullPath);
    if (iter == m_CsbObjectPool.end())
    {
        return;
    }

    LOGDEBUG("performance: CNewCsbLoader unload %s", fullPath.c_str());

    // �ͷ�CsbObject
    delete iter->second;
    m_CsbObjectPool.erase(iter);

    // ��ס��m_DataObjectPool�Ĳ���
    m_DataObjectMutex.lock();
    auto itDataPool = m_DataObjectPool.find(fullPath);
    if (itDataPool != m_DataObjectPool.end())
    {
        if (itDataPool->second->getReferenceCount() <= 1)
        {
            itDataPool->second->release();
            m_DataObjectPool.erase(itDataPool);
        }
    }
    m_DataObjectMutex.unlock();
}

void CNewCsbLoader::cacheRes(const std::string& resName)
{
    string fullPath = FileUtils::getInstance()->fullPathForFilename(resName);
    if (m_CacheRes.find(fullPath) == m_CacheRes.end())
    {
        m_CacheRes.insert(fullPath);
    }
}

void CNewCsbLoader::clearRes()
{
    IResLoader::clearRes();
    // ��ֹͣ����
    if (m_bIsLoading)
    {
        // ��Ϊ��ǿ���жϣ�������Ҫ��ջص�
        m_finishCallback = nullptr;
        onLoadFinish();
    }

    // ��������첽���������е�CSB�����Ǿ��������ذ�...
    // ����Ҫ�����ص�������������ܵ��õ�
    for (auto iter : m_LoadingTextureCsb)
    {
        iter->Callback = nullptr;
    }

    for (auto iter = m_CsbObjectPool.begin();iter != m_CsbObjectPool.end();)
    {
        auto cacheIter = m_CacheRes.find(iter->first);
        if (cacheIter == m_CacheRes.end())
        {
            LOGDEBUG("performance: CNewCsbLoader unload %s", iter->first.c_str());
            // ��ס��m_DataObjectPool�Ĳ���
            m_DataObjectMutex.lock();
            auto itDataPool = m_DataObjectPool.find(iter->first);
            if (itDataPool != m_DataObjectPool.end())
            {
                if (itDataPool->second->getReferenceCount() <= 1)
                {
                    itDataPool->second->release();
                    m_DataObjectPool.erase(itDataPool);
                }
            }
            m_DataObjectMutex.unlock();

            delete iter->second;
            m_CsbObjectPool.erase(iter++);
        }
        else
        {
            ++iter;
        }
    }

    m_DataObjectMutex.lock();
    for (auto itDataPool = m_DataObjectPool.begin();
        itDataPool != m_DataObjectPool.end(); )
    {
        if (itDataPool->second->getReferenceCount() <= 1)
        {
            itDataPool->second->release();
            m_DataObjectPool.erase(itDataPool++);
        }
        else
        {
            ++itDataPool;
        }
    }
    m_DataObjectMutex.unlock();

    timeline::ActionTimelineCache::getInstance()->purge();
    m_CacheRes.clear();
}

void CNewCsbLoader::waitForQuit()
{
    m_bNeedQuit = true;
    if (m_LoadingThread)
    {
        m_LoadingThread->join();
        delete m_LoadingThread;
        m_LoadingThread = nullptr;
    }
    // ǰ���Ѿ����̸߳ɵ���...�������ﲻ�õ����̰߳�ȫ
    deleteAndClearDeque(m_RequestQueue);
    for (auto request : m_ResponseQueue)
    {
        if (request->MainCsbObject)
        {
            delete (request->MainCsbObject);
        }
        request->release();
    }
    m_ResponseQueue.clear();

}

Node* CNewCsbLoader::createCsbNode(const std::string& csbFile)
{
    string fullPath = FileUtils::getInstance()->fullPathForFilename(csbFile);
    CDataObject* dataObj = getDataForCsbFile(fullPath);
    if (dataObj != nullptr)
    {
        auto csparsebinary = GetCSParseBinary(dataObj->getData().getBytes());
        auto textures = csparsebinary->textures();
        int textureSize = csparsebinary->textures()->size();
        for (int i = 0; i < textureSize; ++i)
        {
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile(textures->Get(i)->c_str());
        }

        //CCLOG("CSB %s CREATE START ==================================", csbFile.c_str());
        auto nodetree = csparsebinary->nodeTree();
        auto csbNode = nodeWithFlatBuffers(nodetree);
        //CCLOG("CSB %s CREATE END ==================================", csbFile.c_str());
        if (csbNode)
        {
            size_t pos1 = csbFile.find_last_of("/") + 1;
            size_t pos2 = csbFile.find_last_of(".");
            csbNode->setName(csbFile.substr(pos1, pos2 - pos1));

            // ִ��Action
            auto act = cocostudio::timeline::ActionTimelineCache::getInstance(
                )->createActionWithDataBuffer(dataObj->getData(), csbFile);
            if (act)
            {
                csbNode->runAction(act);
                act->gotoFrameAndPause(0);
            }
        }
        else
        {
            CCLOG("create Csb Node %s Faile", csbFile.c_str());
        }
        return csbNode;
    }

    CCLOG("getDataForCsbFile %s Faile", fullPath.c_str());
    return nullptr;
}

void CNewCsbLoader::loadCsb()
{
    LoadingCsbObject* pLoadingCsb = nullptr;
    //std::mutex signalMutex;
    //std::unique_lock<std::mutex> signal(signalMutex);

    while (!m_bNeedQuit)
    {
        // ��ȡ��Ҫ���ص�csb��Ϣ
        m_RequestMutex.lock();
        if (m_RequestQueue.empty())
        {
            pLoadingCsb = nullptr;
        }
        else
        {
            pLoadingCsb = m_RequestQueue.front();
            m_RequestQueue.pop_front();
        }
        m_RequestMutex.unlock();
        // �����ȡ�������͵ȵ�
        if (nullptr == pLoadingCsb)
        {
            break;
        }

        CDataObject* mainData = getDataForCsbFile(pLoadingCsb->CsbFilePath);
        if (mainData != nullptr)
        {
            // �������
            pLoadingCsb->MainCsbObject = new CsbObject();
            if (pLoadingCsb->MainCsbObject->DataObjects.find(mainData)
                == pLoadingCsb->MainCsbObject->DataObjects.end())
            {
                mainData->retain();
                pLoadingCsb->MainCsbObject->DataObjects.insert(mainData);
            }

            // �Զ�����csb�Լ�Ƕ�׵���csb�����������������������
            // ��������Csb�����ͼƬ��Դ
            searchTexturesByCsbFile(mainData->getData(), pLoadingCsb->TextureFiles);
            auto csparsebinary = GetCSParseBinary(mainData->getData().getBytes());
            // �ٶԸ�Csb�����е��ӽڵ���еݹ飬����Ƕ�׵�Csb��������Ƕ��Csb����Դ
            searchTexturesByCsbNodeTree(csparsebinary->nodeTree(),
                pLoadingCsb->TextureFiles, pLoadingCsb->MainCsbObject->DataObjects);
        }

        m_ResponseMutex.lock();
        m_ResponseQueue.push_back(pLoadingCsb);
        m_ResponseMutex.unlock();
    }
}

// ���ҵ���Csb�����õ�����PNG
void CNewCsbLoader::searchTexturesByCsbFile(const Data& data, set<string>& texSet)
{
    auto csparsebinary = GetCSParseBinary(data.getBytes());
    auto textures = csparsebinary->textures();
    int textureSize = csparsebinary->textures()->size();
    for (int i = 0; i < textureSize; ++i)
    {
        string plistFile = FileUtils::getInstance()->fullPathForFilename(textures->Get(i)->c_str());
        string texFile = getTextFromPlist(plistFile);
        if (!texFile.empty())
        {
            texSet.insert(texFile);
        }
    }
}

void CNewCsbLoader::searchTexturesByCsbNodeTree(const flatbuffers::NodeTree* tree, set<string>& texSet, std::set<CDataObject*>& objects)
{
    // �����е��ӽڵ�����ͬ�Ĵ���
    auto children = tree->children();
    int size = children->size();
    for (int i = 0; i < size; ++i)
    {
        auto subNodeTree = children->Get(i);
        // ����CsbNode�ӽڵ㣬��Ҫһ�����ؽ���
        auto options = subNodeTree->options();
        std::string classname = subNodeTree->classname()->c_str();
        if (classname == "ProjectNode")
        {
            auto projectNodeOptions = (ProjectNodeOptions*)options->data();
            std::string filePath = FileUtils::getInstance()->fullPathForFilename(
                projectNodeOptions->fileName()->c_str());

            // �д��ļ�����δ���ع����ļ�
            // ����Ѿ�����������û��Ҫ������
            if (!filePath.empty())
            {
                auto dataObj = getDataForCsbFile(filePath);
                if (dataObj != nullptr)
                {
                    // �������
                    if (objects.find(dataObj) == objects.end())
                    {
                        dataObj->retain();
                        objects.insert(dataObj);
                    }

                    // �ҵ����Csb�����õ�Png
                    searchTexturesByCsbFile(dataObj->getData(), texSet);
                    auto csparsebinary = GetCSParseBinary(dataObj->getData().getBytes());
                    // �Ը�Csb���еݹ�
                    searchTexturesByCsbNodeTree(csparsebinary->nodeTree(), texSet, objects);
                }
            }
        }
        else
        {
            searchTexturesByCsbNodeTree(subNodeTree, texSet, objects);
        }
    }
}

CDataObject* CNewCsbLoader::getDataForCsbFile(const std::string& csbFile)
{
    CDataObject* ret = nullptr;

    m_DataObjectMutex.lock();
    auto it = m_DataObjectPool.find(csbFile);
    if (it != m_DataObjectPool.end())
    {
        ret = it->second;
    }
    m_DataObjectMutex.unlock();

    if (ret == nullptr)
    {
        auto buf = FileUtils::getInstance()->getDataFromFile(csbFile);
        if (!buf.isNull())
        {
            ret = new CDataObject();
            ret->setData(buf);
            m_DataObjectMutex.lock();
            m_DataObjectPool[csbFile] = ret;
            m_DataObjectMutex.unlock();
        }
    }

    return ret;
}

const std::string& CNewCsbLoader::getTextFromPlist(const std::string& plist)
{
    auto iter = m_PlistTextMap.find(plist);
    if (iter != m_PlistTextMap.end())
    {
        return iter->second;
    }

    Data plistData = FileUtils::getInstance()->getDataFromFile(plist);
    if (plistData.isNull())
    {
        m_PlistTextMap[plist] = "";
    }
    else
    {
        string textureFile;
        ValueMap dict = FileUtils::getInstance()->getValueMapFromData(
            reinterpret_cast<const char*>(plistData.getBytes()), plistData.getSize());

        if (dict.find("metadata") != dict.end())
        {
            ValueMap& metadataDict = dict["metadata"].asValueMap();
            textureFile = metadataDict["textureFileName"].asString();
        }

        if (!textureFile.empty())
        {
            // �������·������������ļ�����Ӧ��plist��·����
            textureFile = FileUtils::getInstance()->fullPathFromRelativeFile(textureFile, plist);
        }
        else
        {
            // ���plist�ļ���û������·���������Զ�ȡplist��Ӧ��.png
            textureFile = plist;
            // ��xxxx.plist��β��.plist�Ƴ����滻��.png
            textureFile = textureFile.erase(textureFile.find_last_of("."));
            textureFile = textureFile.append(".png");
        }
        m_PlistTextMap[plist] = textureFile;
    }
    return m_PlistTextMap[plist];
}

void CNewCsbLoader::onLoadFinish()
{
    waitForQuit();

    Director::getInstance()->getScheduler()->unschedule(
        CC_SCHEDULE_SELECTOR(CNewCsbLoader::addImageAsyncCallBack), this);

    LOGDEBUG("performance: CNewCsbLoader onLoadFinish %d", utils::getTimeInMilliseconds());
    if (m_finishCallback)
    {
        m_finishCallback(m_CurrentLoadedCount, m_TotalLoadingCount);
    }

    m_bIsLoading = false;
    m_CurrentLoadedCount = 0;
    m_TotalLoadingCount = 0;
    autoLoadRes();
}

void CNewCsbLoader::addImageAsyncCallBack(float dt)
{
    while (true)
    {
        size_t sz = m_ResponseQueue.size();
        LoadingCsbObject* pLoadingCsb = nullptr;
        // 1. ��ȡ�����õ�Csb������Ϣ����
        m_ResponseMutex.lock();
        if (m_ResponseQueue.empty())
        {
            pLoadingCsb = nullptr;
        }
        else
        {
            pLoadingCsb = m_ResponseQueue.front();
            m_ResponseQueue.pop_front();
        }
        m_ResponseMutex.unlock();
        if (nullptr == pLoadingCsb)
        {
            break;
        }

        // 2. ��ʼ����Csb������������
        // ����ʧ��
        bool success = false;
        if (pLoadingCsb->MainCsbObject != nullptr)
        {
            // ���سɹ������û��������Ҫ���أ���ֱ����ɣ�����ʼ��������
            if (pLoadingCsb->TextureFiles.size() > 0)
            {
                pLoadingCsb->retain();
                m_LoadingTextureCsb.insert(pLoadingCsb);
                auto itPngFile = pLoadingCsb->TextureFiles.begin();
                for (; itPngFile != pLoadingCsb->TextureFiles.end(); ++itPngFile)
                {
                    string textureFile = *itPngFile;
                    KXLOGDEBUG("csblink %s -- %s", pLoadingCsb->CsbFilePath.c_str(), textureFile.c_str());
                    Director::getInstance()->getTextureCache()->addImageAsync(*itPngFile,
                        [this, textureFile, pLoadingCsb](Texture2D* tex)->void
                    {
                        if (tex == nullptr)
                        {
                            pLoadingCsb->FaileCount++;
                            CCLOG("CsbLoader Load Texture %s Faile", textureFile.c_str());
                        }
                        else
                        {
                            pLoadingCsb->SuccessCount++;
                            // ���ã���֤�����ͷ�
                            pLoadingCsb->MainCsbObject->Textures.insert(tex);
                            tex->retain();
                        }

                        // ���е������������
                        if (pLoadingCsb->SuccessCount + pLoadingCsb->FaileCount >= 
                            static_cast<int>(pLoadingCsb->TextureFiles.size()))
                        {
                            onCsbLoadFinish(pLoadingCsb, pLoadingCsb->FaileCount == 0);
                            m_LoadingTextureCsb.erase(pLoadingCsb);
                        }
                    });
                }
                pLoadingCsb->release();
                // ����Ҫ���أ���������
                continue;
            }
            else
            {
                success = true;
            }
        }

        // 3. �ڼ��������������ִ�лص�������ӵ�m_CsbObjectPool��
        onCsbLoadFinish(pLoadingCsb, success);
    }
}

void CNewCsbLoader::onCsbLoadFinish(LoadingCsbObject* loadingCsb, bool success)
{
    if (success)
    {
        m_CsbObjectPool[loadingCsb->CsbFilePath] = loadingCsb->MainCsbObject;
    }
    else if (loadingCsb->MainCsbObject != nullptr)
    {
        delete loadingCsb->MainCsbObject;
        loadingCsb->MainCsbObject = nullptr;
    }

    if (loadingCsb->Callback != nullptr)
    {
        loadingCsb->Callback(loadingCsb->CsbFilePath, success);
    }

    loadingCsb->release();
    ++m_CurrentLoadedCount;
    if (m_CurrentLoadedCount == m_TotalLoadingCount)
    {
        onLoadFinish();
    }
}

// ����ֻ�������߳��е��ã����������һ��δ����Ԥ���ص�csb
// ���Զ�������Data�������ڼ��ٴ�����������Ϊ������CsbObject����
cocos2d::Node* CNewCsbLoader::nodeWithFlatBuffers(const flatbuffers::NodeTree *nodetree)
{
    Node* node = nullptr;

    std::string classname = nodetree->classname()->c_str();
    auto options = nodetree->options();

    if (classname == "ProjectNode")
    {
        auto reader = ProjectNodeReader::getInstance();
        auto projectNodeOptions = (ProjectNodeOptions*)options->data();
        std::string filePath = FileUtils::getInstance()->fullPathForFilename(
            projectNodeOptions->fileName()->c_str());
        cocostudio::timeline::ActionTimeline* action = nullptr;

        if (filePath != "" && FileUtils::getInstance()->isFileExist(filePath))
        {
            // ��Cache�л�ȡ�������Զ�������Cache
            CDataObject* dataObj = getDataForCsbFile(filePath);
            if (dataObj != nullptr)
            {
                auto csparsebinary = GetCSParseBinary(dataObj->getData().getBytes());
                auto textures = csparsebinary->textures();
                int textureSize = csparsebinary->textures()->size();
                for (int i = 0; i < textureSize; ++i)
                {
                    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(textures->Get(i)->c_str());
                }
                auto nodetree = csparsebinary->nodeTree();
                node = nodeWithFlatBuffers(nodetree);
                if (node == nullptr)
                {
                    return nullptr;
                }
                action = cocostudio::timeline::ActionTimelineCache::getInstance(
                    )->createActionWithDataBuffer(dataObj->getData(), filePath);
            }
            else
            {
                node = Node::create();
            }
        }
        else
        {
            node = Node::create();
        }
        reader->setPropsWithFlatBuffers(node, options->data());
        if (action)
        {
            action->setTimeSpeed(projectNodeOptions->innerActionSpeed());
            node->runAction(action);
            action->gotoFrameAndPause(0);
        }
    }
    else if (classname == "SimpleAudio")
    {
        node = Node::create();
        auto reader = ComAudioReader::getInstance();
        Component* component = reader->createComAudioWithFlatBuffers(options->data());
        if (component)
        {
            node->addComponent(component);
            reader->setPropsWithFlatBuffers(node, options->data());
        }
    }
    else
    {
        std::string customClassName = nodetree->customClassName()->c_str();
        if (customClassName != "")
        {
            classname = customClassName;
        }
        std::string readername = getGUIClassName(classname);
        readername.append("Reader");
        NodeReaderProtocol* reader = dynamic_cast<NodeReaderProtocol*>(
            ObjectFactory::getInstance()->createObject(readername));
        if (reader)
        {
            node = reader->createNodeWithFlatBuffers(options->data());
        }
    }

    if (!node)
    {
        return nullptr;
    }

    auto children = nodetree->children();
    int size = children->size();
    for (int i = 0; i < size; ++i)
    {
        auto subNodeTree = children->Get(i);
        Node* child = nodeWithFlatBuffers(subNodeTree);
        if (child)
        {
            PageView* pageView = dynamic_cast<PageView*>(node);
            ListView* listView = dynamic_cast<ListView*>(node);
            if (pageView)
            {
                Layout* layout = dynamic_cast<Layout*>(child);
                if (layout)
                {
                    pageView->addPage(layout);
                }
            }
            else if (listView)
            {
                Widget* widget = dynamic_cast<Widget*>(child);
                if (widget)
                {
                    listView->pushBackCustomItem(widget);
                }
            }
            else
            {
                node->addChild(child, i);
            }
        }
    }
    return node;
}

string CNewCsbLoader::getGUIClassName(const std::string &name)
{
    std::string convertedClassName = name;
    if (name == "Panel")
    {
        convertedClassName = "Layout";
    }
    else if (name == "TextArea")
    {
        convertedClassName = "Text";
    }
    else if (name == "TextButton")
    {
        convertedClassName = "Button";
    }
    else if (name == "Label")
    {
        convertedClassName = "Text";
    }
    else if (name == "LabelAtlas")
    {
        convertedClassName = "TextAtlas";
    }
    else if (name == "LabelBMFont")
    {
        convertedClassName = "TextBMFont";
    }

    return convertedClassName;
}