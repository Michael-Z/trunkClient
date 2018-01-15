#include "ResPool.h"
#include "ResManager.h"

/*/ ����
1. ������Ѿ����ͷ��ˣ�onExit������
2. ���ã���������һ��ʹ��������������Ϣ���������������ţ����ֿ��ܱ��ı����Ϣ������Ϊ��ʼ״̬
3. ������Node ActionTimeline����ʼ��
*/

USING_NS_CC;

#define MAX_CACHE_SIZE 32

CResPool* CResPool::m_Instance = NULL;

static int spineCount = 0;
static int csbCount = 0;

CResPool::CResPool()
: m_NodeCacheSize(MAX_CACHE_SIZE)
, m_SpineCacheSize(MAX_CACHE_SIZE)
{
}

CResPool::~CResPool()
{
    // �ͷų���
    for (auto& csbs : m_NodeCache)
    {
        for (auto& csb : csbs.second)
        {
            csb->release();
        }
    }
    m_NodeCache.clear();
    for (auto& spines : m_SpineCache)
    {
        for (auto& spineNode : spines.second)
        {
            spineNode->release();
        }
    }
    m_SpineCache.clear();
}

CResPool* CResPool::getInstance()
{
	if (NULL == m_Instance)
	{
		m_Instance = new CResPool();
	}
	return m_Instance;
}

void CResPool::destory()
{
	if (NULL != m_Instance)
	{
		delete m_Instance;
		m_Instance = NULL;
	}
}

Node* CResPool::getCsbNode(const std::string& resName)
{
    // 1. �ӳ�������
    Node* ret = nullptr;
    auto nodeList = m_NodeCache.find(resName);
    if (nodeList != m_NodeCache.end())
    {
        if (nodeList->second.size() > 0)
        {
            ret = *(nodeList->second.begin());
			nodeList->second.erase(nodeList->second.begin());
            ret->autorelease();
        }
    }

    if (ret == nullptr)
    {
        ret = CResManager::getInstance()->cloneCsbNode(resName);
        if (ret == nullptr)
        {
            LOG("create csb %s faile", resName.c_str());
            return nullptr;
        }
    }

    LOG("create csb count %d", ++csbCount);
    // 2. ��ʼ��
    ret->setPosition(Vec2::ZERO);
    ret->setScale(1.0f);
    ret->setVisible(true);
    ret->setOpacity(255);
    ret->setColor(Color3B::WHITE);
    ret->setLocalZOrder(0);
    if (ret->getActionByTag(ret->getTag()) == nullptr)
    {
        auto act = cocostudio::timeline::ActionTimelineCache::getInstance(
            )->createAction(resName);
        if (act)
        {
            ret->runAction(act);
            act->gotoFrameAndPause(0);
        }
    }
    return ret;
}

void CResPool::freeCsbNode(const std::string& resName, cocos2d::Node* node)
{
	if (m_NodeCacheSize > m_NodeCache[resName].size())
    {
        node->retain();
        m_NodeCache[resName].push_back(node);
    }
}

spine::SkeletonAnimation* CResPool::createSpine(const std::string& resName)
{
	// 1. �ӳ�������
	spine::SkeletonAnimation* ret = nullptr;
	
	auto nodeList = m_SpineCache.find(resName);
	if (nodeList != m_SpineCache.end())
	{
		//CCLOG("%s   NOW spinecache size is %d", resName, nodeList->second.size());
		if (nodeList->second.size() > 0)
		{
			ret = *(nodeList->second.begin());
			ret->autorelease();
			nodeList->second.erase(nodeList->second.begin());
		}
	}

	if (ret == nullptr)
	{
		ret = CResManager::getInstance()->createSpine(resName);
		if (ret == nullptr)
		{
			LOG("create spine %s faile", resName.c_str());
			return nullptr;
		}
	}

	// 2. ��ʼ��
    ret->setToSetupPose();
	ret->setTimeScale(1.0f);
	ret->setPosition(Vec2::ZERO);
	ret->setScale(1.0f);
	ret->setVisible(true);
	ret->setOpacity(255);
	ret->setColor(Color3B::WHITE);
	ret->setLocalZOrder(0);
	return ret;
}

void CResPool::freeSpineAnimation(const std::string& resName, spine::SkeletonAnimation* node)
{
	if (m_SpineCacheSize > m_SpineCache[resName].size())
	{
		node->retain();
		m_SpineCache[resName].push_back(node);
	}
}
