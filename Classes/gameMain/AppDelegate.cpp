#include "AppDelegate.h"

#include "CCLuaEngine.h"
#include "HelloWorldScene.h"
#include "GameNetworkNode.h"
#include "LogFileHandler.h"
#include "LogConsoleHandler.h"

#include "LogUiHandler.h"

#include "KxDebuger.h"
#include "LogCocosHandler.h"
#include "LogFileHandler.h"
#include "LogBattleHandler.h"
#include "FMODAudioEngine.h"

#include "Game.h"
#include "CCLuaEngine.h"
#include "lua_summoner_module_register.h"
#include "LuaSummonerBase.h"
#include "LuaSummonerExtend.h"
#include "LuaConfigFunc.h"
#include "LuaBufferData.h"
#include "LuaResManager.h"
#include "LuaUserModel.h"
#include "LuaBagModel.h"
#include "LuaEquipModel.h"
#include "LuaSummonersModel.h"
#include "LuaStageModel.h"
#include "LuaHeroCardBagModel.h"
#include "LuaHeroCardModel.h"
#include "LuaRoomModel.h"
#include "LuaGameModel.h"
#include "LuaTeamModel.h"
#include "LuaTaskModel.h"
#include "LuaAchieveModel.h"
#include "LuaGuideModel.h"
#include "LuaUnionModel.h"
#include "LuaShake.h"
#include "LuaActivityInstanceModel.h"
#include "LuaMailModel.h"
#include "LuaGoldTestModel.h"
#include "LuaHeroTestModel.h"
#include "LuaPersonalTaskModel.h"
#include "LuaTeamTaskModel.h"
#include "LuaTowerTestModel.h"
#include "LuaPvpModel.h"
#include "LuaShopModel.h"
#include "LuaSuperRichText.h"
#include "LuaOperateActiveModel.h"
#include "LuaFMODAudioEngine.h"
#include "LuaHeadModel.h"
#include "../cocos/network/HttpClient.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "CsvCheck.h"
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "QQHallManager.h"
#else
#include "extern/qqHall/QQHallManager.h"
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#   include "bugly/CrashReport.h"
#   include "bugly/lua/BuglyLuaAgent.h"
#   include <android/log.h>
#   include <jni.h>
#   include "platform/android/jni/JniHelper.h"
#   if (ANYSDK)
#   include "../../extern/anysdk/anysdkbindings.h"
#   include "../../extern/anysdk/anysdk_manual_bindings.h"
#   endif
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#   include "CrashReport.h"
#   include "BuglyLuaAgent.h"
#   include <signal.h>
#   include "anysdkbindings.h"
#   include "anysdk_manual_bindings.h"
#endif

#include "ConfManager.h"

USING_NS_CC;
using namespace network;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
extern HICON hdicon;
#endif

AppDelegate::AppDelegate()
{
// #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
// 	CSDKManager::getInstance()->init();
// #endif
    //UserDefault::getInstance()->setIntegerForKey("Online", 0);
}

AppDelegate::~AppDelegate()
{
    /*int loginTime = UserDefault::getInstance()->getIntegerForKey("LoginTime", 0);
    if (CQQHallManager::GetInstance()->isQQHall() && loginTime != 0)
    {
        std::string uidStr = String::createWithFormat("%d", CGame::getInstance()->UserId)->getCString();
        std::string opopenidStr = String::createWithFormat("%d"
            , CQQHallManager::GetInstance()->getCmdLineID().c_str())->getCString();
        std::string onlineTime = String::createWithFormat("%d", time(0) - loginTime)->getCString();

        std::string url = "http://tencentlog.com/stat/report_quit.php";
        std::string pram = "?appid=1105897582&domain=10&opuid=" + uidStr + "&opopenid=" + opopenidStr
            + "&onlinetime=" + onlineTime;

        LOGDEBUG("%s%s", url.c_str(), pram.c_str());

        HttpRequest* request = new (std::nothrow) HttpRequest();
        request->setUrl(url + pram);
        request->setRequestType(HttpRequest::Type::GET);
        HttpClient::getInstance()->sendImmediate(request);
        request->release();
    }*/
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

    GLView::setGLContextAttrs(glContextAttrs);
}

/*void myencrypt(char* data, unsigned int len, int key)
{
    unsigned int maxLen = 256 / sizeof(int);
    len /= sizeof(int);
    for (unsigned int i = 0; i < len && i < maxLen; ++i)
    {
        *(int*)data ^= key;
        data += sizeof(int);
    }
}

class MyFileDelegate : public FileDelegate
{
    virtual Data fileProcess(const std::string& file, Data& data)
    {
        if (FileUtils::getInstance()->getFileExtension(file) == ".png")
        {
            myencrypt((char*)data.getBytes(), data.getSize(), 1314666);
        }
        return data;
    }
};*/

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
        glview = GLViewImpl::createWithRect(gb23122utf8("�ٻ�ʦOL"), Rect(0, 0, 1136.0f, 640), 1.0f, false);
        if (!glview)
        {
            cocos2d::MessageBox(gb23122utf8("��������ʧ��, �볢�����°�װ�Կ�����").c_str(), "glview null");
        }
#else
        glview = GLViewImpl::create("SummonerOL");
#endif
        director->setOpenGLView(glview);
    }
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    glview->setFrameSize(1136.0f, 640.0f);
#endif
    director->getOpenGLView()->setDesignResolutionSize(960.0f, 640.0f, ResolutionPolicy::FIXED_HEIGHT);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    //�򴰿ڷ�����Ϣ������ͼ��
    SendMessage(glview->getWin32Window(), WM_SETICON, ICON_SMALL, (LPARAM)hdicon);
#endif
    Image::setPVRImagesHavePremultipliedAlpha(true);
    // turn on display FPS
    // director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60.0f);

    // ��ʼ���ű�����
    LuaEngine* engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);

    //����bugly��ʼ����Android��iOS��Ҫ���ֲ�ͬ��appid
    // ע���ʼ��Bugly����Ҫ��ִ��engine->executeScriptFile("src/main.lua")֮ǰ
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    CrashReport::initCrashReport("900030181", false);
    BuglyLuaAgent::registerLuaExceptionHandler(engine);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    CrashReport::initCrashReport("900030718", false);
    BuglyLuaAgent::registerLuaExceptionHandler(engine);
#endif

    FileUtils::getInstance()->setPopupNotify(false);
   // android
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
   
   FileUtils::getInstance()->addSearchPath("stage");
   FileUtils::getInstance()->addSearchPath("scripts");
#endif
   // �ȸ���
   std::string storagePath = FileUtils::getInstance()->getWritablePath() + "summonerUpdate";
   FileUtils::getInstance()->addSearchPath(storagePath + "/stage", true);
   FileUtils::getInstance()->addSearchPath(storagePath + "/scripts", true);

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    //ios
    FileUtils::getInstance()->addSearchPath("res");
    FileUtils::getInstance()->addSearchPath("res/stage");
    FileUtils::getInstance()->addSearchPath("res/scripts");
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    //bin
    FileUtils::getInstance()->addSearchPath("../res");
    FileUtils::getInstance()->addSearchPath("../res/stage");
    FileUtils::getInstance()->addSearchPath("../res/scripts");
    //debug
    FileUtils::getInstance()->addSearchPath("../../res");
    FileUtils::getInstance()->addSearchPath("../../res/stage");
    FileUtils::getInstance()->addSearchPath("../../res/scripts");
#endif
    
    // ��ʼ����Ϸ����
    CGame::getInstance()->init();
    KXLOGDEBUG("Game Init");

    //��ʼ������
    CGameNetworkNode *gameNetwork = CGameNetworkNode::getInstance();
    director->setNotificationNode(gameNetwork); 

    LuaStack* stack = engine->getLuaStack();
    stack->setXXTEAKeyAndSign("fanhougame_zhs", strlen("fanhougame_zhs"), "zhs", strlen("zhs"));

    lua_State* L = engine->getLuaStack()->getLuaState();
    lua_summoner_module_register(L);
    regiestSummonerBase();
    regiestSummonerExtend();
    regiestConfigFuncs();
    regiestBufferData();
    registerResManager();

    registeUserModel();
    registeBagModel();
    registeEquipModel();
    registeSummonersModel();
    regiestStageModel();
    registeHeroCardModel();
    registeHeroCardBagModel();
    registerPlayerModel();
    registerSettleAccountModel();
    registerRoomModel();
    regiestGameModel();
    registeTeamModel();

    registeTaskModel();
    registeAchieveModel();
    registerGuideModel();
    registeUnionModel();
    regiestShake();
    registerActivityModel();
    registeMailModel();
    regiestGoldTestModel();
    registerHeroTestModel();
    registePersonalTaskModel();
    registeTeamTaskModel();
    registerTowerTestModel();
    registerPvpModel();
    registeShopModel();
    registeSuperRichText();
    registeOperateActiveModel();
    registeFMODAudioEngine();
    registeHeadModel();

    // �����־
    KxLogger::getInstance()->addHandler(1, new CLogCocosHandler());
	//KxLogger::getInstance()->addHandler(3, new CLogUiHandler());
    KxLogger::getInstance()->setShowDate(false);
    KxLogger::getInstance()->setShowTime(false);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    KxLogFileHandler* fileHandle = new KxLogFileHandler();
    fileHandle->setFileName("ClientDebug");
    fileHandle->setFastModel(false);
    KxLogger::getInstance()->addHandler(2, fileHandle);

    CLogBattleHandler* battleHandle = new CLogBattleHandler();
    battleHandle->setFileName("ConformityClient");
    battleHandle->setFastModel(false);
	battleHandle->setTag(1 << 1);
    KxLogger::getInstance()->addHandler(4, battleHandle);

    // ֻ��windows�ų�ʼ��kxDebuger
    kxdebuger::KxDebuger::getInstance()->init(3333);
#endif

    cocos2d::Director::getInstance()->getScheduler()->schedule([](float dt){
        CFMODAudioEngine::getInstance()->update();
    }, this, 0.05f, false, "FMOD_AUDIO_ENGINE");

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    UserDefault::getInstance()->setStringForKey("DeviceModel", "iOS");
    CCLOG("deviceId: %s", UserDefault::getInstance()->getStringForKey("DeviceIdentifier").c_str());
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    std::string className = "org/cocos2dx/summoner/Help";
    auto deviceId = JniHelper::callStaticStringMethod(className, "getFingerPrint");
    auto productModel = JniHelper::callStaticStringMethod(className, "getModel");
    CCLOG("deviceId: %s", deviceId.c_str());
    CCLOG("deviceModel: %s", productModel.c_str());
    UserDefault::getInstance()->setStringForKey("DeviceModel", "Android");
    UserDefault::getInstance()->setStringForKey("DeviceProductModel", productModel);
    UserDefault::getInstance()->setStringForKey("DeviceIdentifier", deviceId);
#endif

#if (ANYSDK)
	// ���AnySDK���
	lua_getglobal(stack->getLuaState(), "_G");
	tolua_anysdk_open(stack->getLuaState());
	tolua_anysdk_manual_open(stack->getLuaState());
	lua_pop(stack->getLuaState(), 1);
#endif

    KXLOGDEBUG("Game Init FINISH");
    LuaEngine::getInstance()->executeScriptFile("summoner.lua");
	//Director::getInstance()->runWithScene(HelloWorld::createScene());
	//CCLOG("Begin csv check!");
	//if (CConfManager::getInstance()->init())
	//{
	//	CsvCheck::getInstance()->checkCsv();
	//}
	//CCLOG("end csv check!");

// #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
// 	CSDKManager::getInstance()->init();
// #endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    if (CGame::getInstance()->isDebug())
    {
        EventListenerKeyboard* kbListener = EventListenerKeyboard::create();
        kbListener->onKeyPressed = [this](EventKeyboard::KeyCode code, Event* event){
            switch (code)
            {
            case cocos2d::EventKeyboard::KeyCode::KEY_F12:
                KXLOGDEBUG(Director::getInstance()->getTextureCache()->getCachedTextureInfo().c_str());
                break;
            }
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(kbListener, 1);
    }
#endif

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    // if you use SimpleAudioEngine, it must be pause
    //SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	CFMODAudioEngine::getInstance()->mixerSuspend();
	Director::getInstance()->stopAnimation();
	checkPush();
	setLuaBackgroundValue(true);
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    // if you use SimpleAudioEngine, it must resume here
    //SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	CFMODAudioEngine::getInstance()->mixerResume();
	Director::getInstance()->startAnimation();
	clearPush();
	setLuaBackgroundValue(false);
#endif
}
