/*
* �ٻ�ʦ���˿ͻ��˹���ͷ�ļ�
* ����ͻ���ר�õ����ݽṹ��ö�ٺͺ�
*
* 2015-2-5 by ��ү
*/
#ifndef __GAME_COMM_H__
#define __GAME_COMM_H__

#include "KxCSComm.h"
#include "BattleModels.h"

#include "ui/UIWidget.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "ui/UIImageView.h"
#include "ui/UILoadingBar.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "ui/UILayout.h"
#include "ui/UIListView.h"
#include "ui/UIPageView.h"
#include "ui/UIScrollView.h"
#include "ui/UIHelper.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using cocos2d::Color4B;
using cocos2d::Label;
using cocos2d::Sprite;
using cocos2d::Sequence;
using cocos2d::ProgressTo;
using cocos2d::ProgressFromTo;
using cocos2d::ProgressTimer;
using cocos2d::RepeatForever;
using cocos2d::LayerColor;
using cocos2d::ui::Widget;
using cocos2d::ui::Text;
using cocos2d::ui::Button;
using cocos2d::ui::ImageView;
using cocos2d::ui::LoadingBar;
using cocos2d::ui::Scale9Sprite;
using cocos2d::ui::EditBox;
using cocos2d::ui::Layout;
using cocos2d::ui::ListView;
using cocos2d::ui::PageView;
using cocos2d::ui::ScrollView;
using cocos2d::ui::Helper;
using cocostudio::GUIReader;
using cocostudio::Armature;

// ս�������Ĳ㼶Zorder
enum BattleLayerZorder
{
	LZ_SCALE = 0,					// ���Ų㼶
	LZ_UI = 1,						// UI��
	LZ_FLASH = 2,					// ������
};

enum ScaleLayerZorder
{
	SLZ_CHANGE = -5,					// �л���
	SLZ_PROSPECT = -4,				// Զ����
	SLZ_BACKGROUND = -3,				// ������
	SLZ_BATTLEBG = -2,				// ս��������
	SLZ_BATTLE0 = -1,				// ս����0
	SLZ_BATTLE1 = 0,				// ս����1
	SLZ_BATTLE2 = 1,				// ս����2
	SLZ_FOREGRUND = 2,				// ǰ����
};


// ս�������Ĳ㼶��ǩ
enum BattleLayerTag
{
	BLT_CHANGE = 10,				// �л���
	BLT_PROSPECT,					// Զ����
	BLT_BACKGROUND,					// ������
    BLT_BATTLEBG,                   // ս������
	BLT_BATTLE,						// ս����
	BLT_FOREGRUND,					// ǰ����
	BLT_UI,							// UI��
	BLT_FLASH,						// ������
	BLT_DRAG,						// �϶���
};

void		setGray(Node* node, bool b);
Node*		getChildByPath(Node* root, std::string path);
Layout*		loadJson(const std::string& file);
Widget*     getWidget(Node* root, const std::string& path);
Text*		getText(Node* root, const std::string& path);
Layout*		getLayout(Node* root, const std::string& path);
Button*     getButton(Node* root, const std::string& path);
ImageView*	getImageView(Node* root, const std::string& path);
LoadingBar* getLoadingBar(Node* root, const std::string& path);
ListView*	getListView(Node* root, const std::string& path);
PageView*	getPageView(Node* root, const std::string& path);
ScrollView* getScrollView(Node* root, const std::string& path);

#endif