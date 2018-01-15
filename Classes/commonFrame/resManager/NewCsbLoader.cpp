#include "NewCsbLoader.h"

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

NewCsbLoader::NewCsbLoader(): m_CurrentLoadedCount(0), m_bIsAutoSerachSubCsb(true)
{
}


NewCsbLoader::~NewCsbLoader()
{
	m_finishCallback = nullptr;
	onLoadFinish();

	for (auto& item : m_CsbNodes)
	{
		item.second->cleanup();
		item.second->release();
	}
	m_CsbNodes.clear();
}

bool NewCsbLoader::addPreloadRes(const std::string& csbFile, const ResLoadedCallback& callBack)
{
	//CCLOG("add csb res %s", csbFile.c_str());
	if (!checkLoadRes(csbFile, "", callBack))
	{
		return false;
	}

	//����csb�ļ������ŵ�Map�У���CSBLoader����
	string fullPath = FileUtils::getInstance()->fullPathForFilename(csbFile);

	if (m_LoadingQueue.find(fullPath) != m_LoadingQueue.end())
	{
		return true;
	}
	m_LoadingQueue[fullPath] = NewCsbLoadingInfo();
	NewCsbLoadingInfo& item = m_LoadingQueue[fullPath];
	item.CsbFilePath = fullPath;
	item.Callback = callBack;
	item.LoadedCount = 0;


	if (m_CheckedCsb.find(fullPath) == m_CheckedCsb.end())
	{
		m_CheckedCsb.insert(fullPath);
		Data data = FileUtils::getInstance()->getDataFromFile(fullPath);
		if (!data.isNull())
		{
			m_CsbFileCache[fullPath] = data;
		}
		else
		{
			CCLOG("no such file %s", fullPath.c_str());
		}
	}

	/************************************************************************/
	/* ɶ������                                                             */
	/************************************************************************/

	//// �������е�ͼƬ��Դ���ѱ����أ�����Ҳ��Ҫ��Csb�ڵ����
	//// ������Csb�Ѿ��������������ټ����
	////1. addPreloadRes��������png��Ҳ�������ļ���ֻ��ͨ�����̡�
	//if (m_CheckedCsb.find(fullPath) == m_CheckedCsb.end())
	//{
	//	m_CheckedCsb.insert(fullPath);
	//	Data data = FileUtils::getInstance()->getDataFromFile(fullPath);
	//	if (!data.isNull())
	//	{
	//		m_CsbFileCache[fullPath] = data;
	//		// ��������Csb�����ͼƬ��Դ
	//		searchTexturesByCsbFile(data, item.PreloadTextures);
	//		if (m_bIsAutoSerachSubCsb)
	//		{
	//			auto csparsebinary = GetCSParseBinary(data.getBytes());
	//			// �ٶԸ�Csb�����е��ӽڵ���еݹ飬����Ƕ�׵�Csb��������Ƕ��Csb����Դ
	//			searchTexturesByCsbNodeTree(csparsebinary->nodeTree(), item.PreloadTextures);
	//		}
	//	}
	//	else
	//	{
	//		CCLOG("no such file %s", fullPath.c_str());
	//	}
	//}

	return true;
}

bool NewCsbLoader::startLoadResAsyn()
{
	// ���ڼ��أ���ȼ������
	// û�ж����ɼ���
	if (m_LoadingQueue.size() == 0 || !IResLoader::startLoadResAsyn())
	{
		return false;
	}

	IResLoader::startLoadResAsyn();
	CSLoader::getInstance();

	m_bIsLoading = true;
	m_CurrentLoadedCount = 0;

	std::function<void(void*)> mainThread = [=](void* param)->void
	{
		// �����е����������첽����
		for (auto& son : m_LoadingQueue)
		{
			auto& item = son.second;
			for (auto& texFile : item.PreloadTextures)
			{
				// ����ǰ���Ѿ������е��ļ���ִ����fullpathforfilename
				// ���������createCsbNodeByLoadedTexture�е�fullpathforfilename���������̰߳�ȫ��
				Director::getInstance()->getTextureCache()->addImageAsync(texFile, [&item](Texture2D* tex)->void
				{
					// ���ﲻʹ�ý�PreloadTextures�Ƴ���ԭ���ǣ��ûص��п��ܱ���������
					// ����ʹ��һ��int�����м���
					++item.LoadedCount;
				});
			}
			m_CurrentLoadedCount++;
		}

		// ע��Schedule�����ڼ�������첽�������
		// ���ﲻʹ��addImageAsync�Ļص�����ΪaddImageAsync���ܱ��������ã���m_LoadingQueue��ǰ��������
		//���ȴ������е�CSB
		//Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(NewCsbLoader::createCsbNodeByLoadedTexture), this, 0, false);
		// ��������
		if (static_cast<unsigned int>(m_CurrentLoadedCount) >= m_LoadingQueue.size())
		{
			onLoadFinish();
		}
	};

	AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_IO, mainThread, (void*)NULL, [=]()
	{
		//�ݹ�������������Ƕ�׵�csb��plist
		for (auto son : m_LoadingQueue)
		{
			auto item = son.second;
			Data data = FileUtils::getInstance()->getDataFromFile(item.CsbFilePath);

			//m_CsbFileCache[item.CsbFilePath] = data;

			// ��������Csb�����ͼƬ��Դ
			searchTexturesByCsbFile(data, item.PreloadTextures);

			auto csparsebinary = GetCSParseBinary(data.getBytes());
			// �ٶԸ�Csb�����е��ӽڵ���еݹ飬����Ƕ�׵�Csb��������Ƕ��Csb����Դ
			searchTexturesByCsbNodeTree(csparsebinary->nodeTree(), item.PreloadTextures);
		}
	});

	
	return true;
}

void NewCsbLoader::removeRes(const std::string& csbFile)
{
	string fullPath = FileUtils::getInstance()->fullPathForFilename(csbFile);
	if (m_bIsLoading)
	{
		CCLOG("thread is loading");
		return;
	}

	auto iter = m_CsbNodes.find(fullPath);
	if (iter == m_CsbNodes.end())
	{
		//CCLOG("can no find such csb node at cache %s", fullPath.c_str());
		return;
	}
	iter->second->cleanup();
	iter->second->release();
	m_CsbNodes.erase(iter);
}

void NewCsbLoader::cacheRes(const std::string& resName)
{
	string fullPath = FileUtils::getInstance()->fullPathForFilename(resName);
	if (m_CacheRes.find(fullPath) == m_CacheRes.end() /*&&
		m_CsbNodes.find(fullPath) != m_CsbNodes.end()*/)
	{
		m_CacheRes.insert(fullPath);
	}

}

void NewCsbLoader::clearRes()
{
	for (std::map<std::string, cocos2d::Node*>::iterator iter = m_CsbNodes.begin();
		iter != m_CsbNodes.end();)
	{
		auto cacheIter = m_CacheRes.find(iter->first);
		if (cacheIter == m_CacheRes.end())
		{
			Node* node = iter->second;
			if (node->getReferenceCount() > 1)
			{
				CCLOG("csb leak %s", iter->first.c_str());
			}
			node->cleanup();
			node->release();
			m_CsbNodes.erase(iter++);
		}
		else
		{
			++iter;
		}
	}

	m_LoadingQueue.clear();
	m_LoadingTextures.clear();
	m_LoadingPlists.clear();
	m_CheckedCsb.clear();
	m_CsbFileCache.clear();

	timeline::ActionTimelineCache::getInstance()->purge();
	m_CacheRes.clear();
	m_AutoLoadResCache.clear();
	IResLoader::clearRes();
}

Node* NewCsbLoader::getCsbNode(const std::string& csbFile)
{
	//ԭ�����Ǵ������� ֱ��ȡ�������� ,���ڻ���Ҫ��ʱ���ٴ��� 
	//string fullPath = FileUtils::getInstance()->fullPathForFilename(csbFile);
	//auto iter = m_CsbNodes.find(fullPath);
	//if (iter == m_CsbNodes.end())
	//{
	//	//CCLOG("can no find such csb node at cache %s", fullPath.c_str());
	//	return nullptr;
	//}
	//return iter->second;

	string fullPath = FileUtils::getInstance()->fullPathForFilename(csbFile);

	if (m_CsbNodes.find(fullPath) != m_CsbNodes.end())
	{
		//����ǰ����������
		return m_CsbNodes[fullPath];
	}

	//�����ٷ���
	return createCsbNode(m_LoadingQueue[fullPath]);
}

void NewCsbLoader::createCsbNodeByLoadedTexture(float dt)
{
	//int count = 0;
	//for (unsigned int i = m_CurrentLoadedCount; i < m_LoadingQueue.size(); ++i)
	//{
	//	auto& item = m_LoadingQueue[i];
	//	if (static_cast<unsigned int>(item.LoadedCount) >= item.PreloadTextures.size()&& count++ < 3)
	//	{
	//		// �����Csb������Դ׼������������д���
	//		createCsbNode(item);
	//		++m_CurrentLoadedCount;
	//	}
	//	else
	//	{
	//		break;
	//	}
	//}
	//
	//// ��������
	//if (static_cast<unsigned int>(m_CurrentLoadedCount) >= m_LoadingQueue.size())
	//{
	//	onLoadFinish();
	//}
}

Node* NewCsbLoader::createCsbNode(const NewCsbLoadingInfo& csbInfo)
{
	// 1.Json������CSLoader����Csb���Լ���Loader�����Ч�ʣ��ݲ�����json��
	// 2.������Դ�Ѿ����أ�������������һЩ����
	// 3.�������߳�ǰ���ļ������Ѿ�Cache�������ƹ���FileUtils���̲߳���ȫ����
	//auto csbNode = CSLoader::getInstance()->createNode(csbInfo.CsbFilePath, nullptr);

	// �Զ���ļ���Csb�ڵ�
	auto iter = m_CsbFileCache.find(csbInfo.CsbFilePath);
	if (iter == m_CsbFileCache.end())
	{
		CCLOG("Csb %s Not Load", csbInfo.CsbFilePath.c_str());
		return nullptr;
	}

	Data buf = iter->second;
	string str = iter->first;
	m_CsbFileCache.erase(iter);
	auto csparsebinary = GetCSParseBinary(buf.getBytes());
	auto textures = csparsebinary->textures();
	int textureSize = csparsebinary->textures()->size();
	for (int i = 0; i < textureSize; ++i)
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(textures->Get(i)->c_str());
	}

	//CCLOG("CSB %s CREATE START ==================================", str.c_str());
	auto nodetree = csparsebinary->nodeTree();
	auto csbNode = nodeWithFlatBuffers(nodetree);
	//CCLOG("CSB %s CREATE END ==================================", str.c_str());
	if (csbNode)
	{
		size_t pos1 = str.find_last_of("/") + 1;
		size_t pos2 = str.find_last_of(".");
		csbNode->setName(str.substr(pos1, pos2 - pos1));
		csbNode->retain();
		m_CsbNodes[csbInfo.CsbFilePath] = csbNode;
	}
	else
	{
		CCLOG("create Csb Node %s Faile", csbInfo.CsbFilePath.c_str());
	}
	//CCLOG("NewCsbLoader::createCsbNode %s", csbInfo.CsbFilePath.c_str());
	if (csbInfo.Callback)
	{
		csbInfo.Callback(csbInfo.CsbFilePath, csbNode != nullptr);
	}

	return csbNode;
}


std::string NewCsbLoader::getGUIClassName(const std::string &name)
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

cocos2d::Node* NewCsbLoader::nodeWithFlatBuffers(const flatbuffers::NodeTree *nodetree)
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

		auto dataIter = m_CsbFileCache.find(filePath);
		if (dataIter != m_CsbFileCache.end())
		{
			Data buf = dataIter->second;
			auto csparsebinary = GetCSParseBinary(buf.getBytes());
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
			action = cocostudio::timeline::ActionTimelineCache::getInstance()->createActionWithFlatBuffersFile(filePath);
		}
		else if (filePath != "" && FileUtils::getInstance()->isFileExist(filePath))
		{
			Data buf = FileUtils::getInstance()->getDataFromFile(filePath);
			auto csparsebinary = GetCSParseBinary(buf.getBytes());
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
				)->createActionWithFlatBuffersFile(filePath);
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
			//action->setTag(TAG_CSB_ACTION);
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

// ���ҵ���Csb�����õ�����PNG
void NewCsbLoader::searchTexturesByCsbFile(Data& data, set<string>& texSet)
{
	auto csparsebinary = GetCSParseBinary(data.getBytes());
	auto textures = csparsebinary->textures();
	int textureSize = csparsebinary->textures()->size();
	for (int i = 0; i < textureSize; ++i)
	{
		string plistFile = FileUtils::getInstance()->fullPathForFilename(textures->Get(i)->c_str());
		if (m_LoadingPlists.find(plistFile) != m_LoadingPlists.end()|| SpriteFrameCache::getInstance()->isSpriteFramesWithFileLoaded(plistFile))
		{
			continue;
		}
		m_LoadingPlists.insert(plistFile);
		Data plistData = FileUtils::getInstance()->getDataFromFile(plistFile);
		if (plistData.isNull())
		{
			continue;
		}

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
			// �������·�������������ļ�����Ӧ��plist��·����
			textureFile = FileUtils::getInstance()->fullPathFromRelativeFile(textureFile, plistFile);
		}
		else
		{
			// ���plist�ļ���û������·���������Զ�ȡplist��Ӧ��.png
			textureFile = plistFile;
			// ��xxxx.plist��β��.plist�Ƴ����滻��.png
			textureFile = textureFile.erase(textureFile.find_last_of("."));
			textureFile = textureFile.append(".png");
		}

		// 
		if (Director::getInstance()->getTextureCache()->getTextureForKey(textureFile) == nullptr
			&& m_LoadingTextures.find(textureFile) == m_LoadingTextures.end())
		{
			m_LoadingTextures.insert(textureFile);
			texSet.insert(textureFile);
		}
	}
}

void NewCsbLoader::searchTexturesByCsbNodeTree(const flatbuffers::NodeTree* tree, set<string>& texSet)
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
			if (!filePath.empty()&& m_CsbNodes.find(filePath) == m_CsbNodes.end()&& m_CheckedCsb.find(filePath) == m_CheckedCsb.end())
			{
				m_CheckedCsb.insert(filePath);
				Data data = FileUtils::getInstance()->getDataFromFile(filePath);
				if (!data.isNull())
				{
					m_CsbFileCache[filePath] = data;
					// �ҵ����Csb�����õ�Png
					searchTexturesByCsbFile(data, texSet);
					auto csparsebinary = GetCSParseBinary(data.getBytes());
					// �Ը�Csb���еݹ�
					searchTexturesByCsbNodeTree(csparsebinary->nodeTree(), texSet);
				}
			}
		}
		else
		{
			searchTexturesByCsbNodeTree(subNodeTree, texSet);
		}
	}
}

void NewCsbLoader::onLoadFinish()
{
	// ȡ��schedule
	//Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(NewCsbLoader::createCsbNodeByLoadedTexture), this);

	//CCLOG("NewCsbLoader::onFinish");
	m_bIsLoading = false;
	// m_CurrentLoadedCount = 0;
	if (m_finishCallback)
	{
		m_finishCallback(m_CurrentLoadedCount, m_LoadingQueue.size());
	}

	// �����˴��첽���ص���ʱ����
	//m_LoadingQueue.clear();
	//m_LoadingTextures.clear();
	//m_LoadingPlists.clear();
	//m_CheckedCsb.clear();
	//m_CsbFileCache.clear();
	autoLoadRes();
}

bool NewCsbLoader::hasRes(const std::string& resName)
{
	return  m_LoadingQueue.find(FileUtils::getInstance()->fullPathForFilename(resName)) != m_LoadingQueue.end();
}