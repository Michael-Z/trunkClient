#include "LogUiHandler.h"
#include "LuaTools.h"
#include "cocostudio/CocoStudio.h"
#include "UILogLayer.h"
#include "LuaBasicConversions.h"

USING_NS_CC;
#define MAX_GAP	100

CLogUiHandler::CLogUiHandler()
{
	this->m_MaxCount = 200;
	this->logLayer = nullptr;
	this->m_isRun = true;
	this->m_isFirst = true;
	this->m_isSecond = false;
	this->m_isThird = false;
	this->m_isFour = false;
	this->m_isSuccessful = false;
	this->m_twoFirst = false;  // �ڶ��׷����õ�����
	auto EventList = EventListenerTouchOneByOne::create();

	EventList->onTouchBegan = [=](Touch* _touch, Event* _event)->bool
	{
		auto po = _touch->getLocation();
		this->prePo = po;
		//��������
		//��һ����
		if (this->m_isFirst && !this->m_isSecond && !this->m_isThird && !this->m_isFour)
		{
			//��һ����
			//CCLOG("11111111111111111111111111111");
			this->m_Point.push_back(po);
		}
		//�ڶ���
		if (this->m_isSecond && !this->m_isFirst && !this->m_isThird && !this->m_isFour)
		{
			//��������
			//CCLOG("3333333333333333333333333333333");
			this->m_Point.push_back(po);
		}
		//������
		if (this->m_isThird && !this->m_isFirst && !this->m_isSecond && !this->m_isFour)
		{
			//�������
			//CCLOG("555555555555555555555555555555555555555555");
			this->m_Point.push_back(po);
		}

		if (this->m_isFour && !this->m_isFirst && !this->m_isSecond && !this->m_isThird)
		{
			//���߸���
			//CCLOG("777777777777777777777777777777777777777");
			this->m_Point.push_back(po);
		}

		return true;
	};
	EventList->onTouchMoved = [=](Touch* _touch, Event* _event){};

	EventList->onTouchEnded = [=](Touch* _touch, Event* _event)
	{
		auto po = _touch->getLocation();
		this->nowPo = po;
		auto distance = this->prePo.getDistance(this->nowPo);
		auto maxVi = Director::getInstance()->getVisibleSize();

		//���Ĵβ���, ���µ�����
		if (prePo.x < MAX_GAP && prePo.y < MAX_GAP && nowPo.x > maxVi.width - MAX_GAP && nowPo.y < MAX_GAP
			&& this->m_isFour && !this->m_isFirst && !this->m_isSecond && !this->m_isThird)
		{
			//CCLOG("888888888888888888888888888");
			//��������� ��¼��4����
			this->m_Point.push_back(po);
			this->m_isSuccessful = true;

			this->m_isSecond = false;
			this->m_isFirst = false;
			this->m_isThird = false;
			this->m_isFour = true;
		}
		//��������ɾ����
		else if (this->m_isFour && !this->m_isFirst && !this->m_isSecond && !this->m_isThird)
		{
			reSet();
		}

		//�����β���, ���ϵ�����
		if (prePo.x<MAX_GAP && prePo.y > maxVi.height - MAX_GAP && nowPo.x > maxVi.width - MAX_GAP && nowPo.y > maxVi.height - MAX_GAP 
			&& this->m_isThird && !this->m_isSecond && !this->m_isFirst && !this->m_isFour)
		{
			//CCLOG("66666666666666666666666");
			//��������� ��¼��4����
			this->m_Point.push_back(po);
			this->m_isSuccessful = false;

			this->m_isSecond = false;
			this->m_isFirst = false;
			this->m_isThird = false;
			this->m_isFour = true;
		}
		//��������ɾ����
		else if (this->m_isThird && !this->m_isFirst && !this->m_isSecond && !this->m_isFour)
		{
			reSet();
		}

		//�ڶ��β���, ���ϵ�����
		if (prePo.x < MAX_GAP && prePo.y > maxVi.height-MAX_GAP && nowPo.x > maxVi.width - MAX_GAP && nowPo.y < MAX_GAP 
			&& this->m_isSecond && !this->m_isFirst && !this->m_isThird && !this->m_isFour)
		{
			//��������� ��¼��4����
			//CCLOG("44444444444444444444444444444444");
			this->m_Point.push_back(po);
			this->m_isSuccessful = false;
			this->m_isSecond = false;
			this->m_isFirst = false;
			this->m_isFour = false;
			this->m_isThird = true;
		}
		//��������ɾ����
		else if (this->m_isSecond && !this->m_isFirst && !this->m_isThird && !this->m_isFour)
		{
			reSet();
		}

		//��һ�β���, ���ϵ�����
		if (prePo.x > maxVi.width - MAX_GAP && prePo.y > maxVi.height-MAX_GAP && nowPo.x < MAX_GAP && nowPo.y < MAX_GAP 
			&& this->m_isFirst && !this->m_isSecond  && !this->m_isThird && !this->m_isFour)
		{
			//CCLOG("2222222222222222222222222222222222222");
			//��������� ��¼�ڶ�����
			this->m_Point.push_back(po);
			this->m_isSuccessful = false;
			this->m_isFirst = false;
			this->m_isThird = false;
			this->m_isFour = false;
			this->m_isSecond = true;
		}
		//��������ɾ����
		else if (this->m_isFirst && !this->m_isSecond && !this->m_isThird && !this->m_isFour)
		{
			reSet();
		}

		if (this->m_isSuccessful && this->m_Point.size() == 8)
		{
			if (!Director::getInstance()->getRunningScene()->getChildByTag(100865566))
			{
				this->logLayer = static_cast<UILogLayer*>(UILogLayer::create());
				Director::getInstance()->getRunningScene()->addChild(this->logLayer, 9999999, 100865566);
			}
			reSet();
		}
	};
	EventList->setSwallowTouches(false);
	auto eventLister = Director::getInstance()->getEventDispatcher();
	eventLister->addEventListenerWithFixedPriority(EventList, -129);

	auto EventList1 = EventListenerTouchOneByOne::create();
	EventList1->onTouchBegan = [=](Touch* _touch, Event* _event)->bool
	{
		auto po = _touch->getLocation();
		this->m_Point2.push_back(po);
		return true;
	};
	EventList1->onTouchMoved = [=](Touch* _touch, Event* _event)
	{
		//��Ϊ10,ֻҪ�����˶�����,��ʮ��Ҫ����
		if (this->m_Point2.size() == 10)
		{
			this->m_Point2.clear();
			//��һ����������
			this->m_twoFirst = true;
		}
		else 
		{
			this->m_Point2.clear();
		}
	};
	EventList1->onTouchEnded = [=](Touch* _touch, Event* _event)
	{
		auto po = _touch->getLocation();
		auto maxVi = Director::getInstance()->getVisibleSize();

		if (this->m_twoFirst)
		{
			this->m_twoFirst = false;
			//�������Ͻ�
			if (po.x < MAX_GAP && po.y >= maxVi.height - MAX_GAP)
			{
				if (!Director::getInstance()->getRunningScene()->getChildByTag(100865566))
				{
					this->logLayer = static_cast<UILogLayer*>(UILogLayer::create());
					Director::getInstance()->getRunningScene()->addChild(this->logLayer, 9999999, 100865566);
				}
			}
		}
	};
	EventList1->setSwallowTouches(false);
	eventLister->addEventListenerWithFixedPriority(EventList1, -130);
}

CLogUiHandler::~CLogUiHandler()
{

}

bool CLogUiHandler::onLog(int level, const std::string& log)
{
	size_t messCount = m_VuiLogMess.size();
	if (messCount > m_MaxCount)
	{
		m_VuiLogMess.erase(m_VuiLogMess.begin());
	}

	m_VuiLogMess.push_back(log);
	if (Director::getInstance()->getRunningScene() 
		&& Director::getInstance()->getRunningScene()->getChildByTag(100865566))
	{
		if (m_isRun && this->logLayer)
		{
			static_cast<UILogLayer*>(this->logLayer)->testMsg(this);
		}
	}
	return true;
}

std::vector<std::string>& CLogUiHandler::getVectorMess()
{
	return m_VuiLogMess;
}

void CLogUiHandler::reSet()
{
	//��ԭ��ʼ״̬
	this->m_isFirst = true;
	this->m_isSecond = false;
	this->m_isThird = false;
	this->m_isFour = false;
	this->m_isSuccessful = false;
	this->m_Point.clear();
}


