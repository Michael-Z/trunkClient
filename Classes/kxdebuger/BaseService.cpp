#include "BaseService.h"
#include "KxCSComm.h"
#include "KxDebugerProtocol.h"

namespace kxdebuger {

BaseService::BaseService()
{

}

BaseService::~BaseService()
{

}

void BaseService::process(int actionId, void* data, int len, KxServer::IKxComm *target)
{
	auto d = Director::getInstance();
	auto s = d->getRunningScene();
	switch (actionId)
	{
	case ActionPauseOrResume:
		{
			if (d->isPaused())
			{
				d->resume();
			}
			else
			{
				d->pause();
			}
			break;
		}
	case ActionStep:
		{
			d->resume();
			float delta = d->getAnimationInterval();		//����һ֡����ͣ
            s->scheduleOnce(CC_SCHEDULE_SELECTOR(BaseService::actionStepSelector), delta);
			break;
		}
	case ActionLogicStep:
	    {
            d->resume();
			float delta = d->getAnimationInterval() * 6;		//������֡����ͣ
            s->scheduleOnce(CC_SCHEDULE_SELECTOR(BaseService::actionStepSelector), delta);
		    break;
	    }
		break;
	default:
		break;
	}
}

void BaseService::actionStepSelector(float dt)
{
    Director::getInstance()->pause();
}

}
