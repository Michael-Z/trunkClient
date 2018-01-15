#ifndef __DISPLAY_COMMON_H__
#define __DISPLAY_COMMON_H__

#include <vector>
#include "cocos2d.h"
#include "CommTools.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/CCComExtensionData.h"
using namespace cocostudio;

int playMusic(int musicId, float delay, float volume, float track, cocos2d::Node* node);
void playSoundEffect(int effectId);
void playUISoundEffect(cocos2d::Node* node);

bool playCsbAnimation(cocos2d::Node* node, const std::string& animation, bool loop = false);
cocostudio::timeline::ActionTimeline* getCsbAnimation(cocos2d::Node* root);
// ��ת��CSB����ָ���İٷֱ�
void gotoCsbAnimationPercent(cocostudio::timeline::ActionTimeline* ani, const char* name, float percent);

template<typename T>
T* findChild(cocos2d::Node* root, const char* path)
{
    T* ret = nullptr;
    if (root != nullptr)
    {
        std::vector<std::string> paths = splitStringToVec(std::string(path), std::string("/"));
        for (auto& name : paths)
        {
            root = root->getChildByName(name);
            if (root == nullptr)
            {
                return nullptr;
            }
        }
        ret = dynamic_cast<T*>(root);
    }
    return ret;
}

// ����Node�����ȫ��ZOrder
void updateGrobleZOrder(Node* node, float zorder);


/** 
@brief  ��cutPanel���ӽڵ㿽����clipper
@param  clipper     �ü��ڵ�
@param  cutPanel    �ü�����(�������������Ҫ�ü��Ľڵ�), ǿ�ҽ���ʹ��Layout
*/
template<typename T>
void setClipperNodeContent(cocos2d::Node* clipper, T* cutPanel)
{
    // ���òü�����
    Vector<Node *> clipperChildren;
    for (auto& node : cutPanel->getChildren())
    {
        clipperChildren.pushBack(node);
        node->retain();
    }
    for (auto& node : clipperChildren)
    {
        node->removeFromParentAndCleanup(false);
        clipper->addChild(node);
        node->release();
    }
}

/**
@brief  ��cutPanel�����Կ�����clipper
@param  clipper     �ü��ڵ�
@param  cutPanel    �ü�����(�������������Ҫ�ü��Ľڵ�), ǿ�ҽ���ʹ��Layout
*/
template<typename T>
void setClipperNodeProperty(cocos2d::Node* clipper, T* cutPanel)
{
    // ���òü��ڵ��������
    clipper->setTag(cutPanel->getTag());
    clipper->setName(cutPanel->getName());
    clipper->ignoreAnchorPointForPosition(cutPanel->isIgnoreAnchorPointForPosition());
    clipper->setAnchorPoint(cutPanel->getAnchorPoint());
    clipper->setPosition(cutPanel->getPosition());
    clipper->setContentSize(cutPanel->getContentSize());
    clipper->setLocalZOrder(cutPanel->getLocalZOrder());
    // ���òü��ڵ��������
    auto com = dynamic_cast<ComExtensionData*>(cutPanel->getComponent(ComExtensionData::COMPONENT_NAME));
    if (com)
    {
        ComExtensionData* extensionData = ComExtensionData::create();
        extensionData->setCustomProperty(com->getCustomProperty());
        extensionData->setActionTag(com->getActionTag());
        if (clipper->getComponent(ComExtensionData::COMPONENT_NAME))
        {
            clipper->removeComponent(ComExtensionData::COMPONENT_NAME);
        }
        clipper->addComponent(extensionData);
    }
}

/**
@brief  clipper�滻cutPanel
@param  clipper     �ü��ڵ�
@param  cutPanel    �ü�����(�������������Ҫ�ü��Ľڵ�), ǿ�ҽ���ʹ��Layout
*/
template<typename T>
void replaceClipperToCutPanel(cocos2d::Node* clipper, T* cutPanel)
{
    if (cutPanel->getParent())
    {
        cutPanel->getParent()->addChild(clipper);
        cutPanel->removeFromParent();
    }
}

#endif
