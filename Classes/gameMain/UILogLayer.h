/***********************************************************     
* �ļ�����: UILogLayer         
* �� �� �ߣ�Mr.Alen     
* �������ڣ�2016/09/18         
/***********************************************************/
#ifndef __UILOG_LAYER_H__
#define __UILOG_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "LogUiHandler.h"
#include "LuaTools.h"
#include "LuaBasicConversions.h"



class UILogLayer : public cocos2d::LayerColor
{
public:
	UILogLayer();
	~UILogLayer();
	static UILogLayer* create();

    virtual bool init();

	void initUI();
	void testMsg(Ref* pSender);

private:
	bool isRun = false;
	ui::ListView* m_ListView = nullptr;
    // ���30��, ����c++����bug, ������¼ (����bugly C++���붨λ)
    int m_nCrashCount;
};


#endif // __UILOG_LAYER_H__
