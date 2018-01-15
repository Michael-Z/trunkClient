#include "QQHallManager.h"
#include "LuaSummonerBase.h"

const char* szDllPath = "QQGameProcMsgHelper.dll";
const char* szCreateClientObjFunc = "CreateClientProcMsgObject";
const char* szReleaseClientObjFunc = "ReleaseClientProcMsgObject";

typedef IClientProcMsgObject* (*CreateObjFunc)();
typedef void(*ReleaseObjFunc)(IClientProcMsgObject*);

static int split(const std::string& str, std::vector<std::string>& ret_, std::string sep = ",")
{
    if (str.empty())
    {
        return 0;
    }

    std::string tmp;
    std::string::size_type pos_begin = str.find_first_not_of(sep);
    std::string::size_type comma_pos = 0;

    while (pos_begin != std::string::npos)
    {
        comma_pos = str.find(sep, pos_begin);
        if (comma_pos != std::string::npos)
        {
            tmp = str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + sep.length();
        }
        else
        {
            tmp = str.substr(pos_begin);
            pos_begin = comma_pos;
        }

        if (!tmp.empty())
        {
            ret_.push_back(tmp);
            tmp.clear();
        }
    }
    return 0;
}

std::wstring StringUtf8ToWideChar(const std::string& strUtf8)
{
    std::wstring ret;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    if (!strUtf8.empty())
    {
        int nNum = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, nullptr, 0);
        if (nNum)
        {
            WCHAR* wideCharString = new WCHAR[nNum + 1];
            wideCharString[0] = 0;

            nNum = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, wideCharString, nNum + 1);

            ret = wideCharString;
            delete[] wideCharString;
        }
        else
        {
            CCLOG("Wrong convert to WideChar code:0x%x", GetLastError());
        }
    }
#endif
    return ret;
}

CQQHallManager *CQQHallManager::m_pInstance = nullptr;
CQQHallManager::CQQHallManager() :m_bIsQQHall(false)
, m_strOpenId("")
, m_strOpenKey("")
, m_strProcPara("")
, m_pProcMsgObj(nullptr)
{
}

CQQHallManager::~CQQHallManager()
{
    if (m_pProcMsgObj)
    {
        ReleaseObj(m_pProcMsgObj);
    }
}

CQQHallManager* CQQHallManager::GetInstance()
{
    if (nullptr == m_pInstance)
    {
        m_pInstance = new CQQHallManager;
    }

    return m_pInstance;
}

void CQQHallManager::Destory()
{
    if (nullptr != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

bool CQQHallManager::Init(wchar_t* cmdLine)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    clearArgs();
    if (nullptr == cmdLine || 0 == wcslen(cmdLine))
    {
        return false;
    }

    // wchar_tתchar
    wchar_t *WStr = cmdLine;
    size_t len = wcslen(WStr) + 1;
    size_t converted = 0;
    char *cmdLineStr;
    cmdLineStr = (char*)malloc(len*sizeof(char));
    wcstombs_s(&converted, cmdLineStr, len, WStr, _TRUNCATE);

    std::string cmdline = cmdLineStr;
    free(cmdLineStr);

    //��һ������������������Ƿ����Ҫ�󣺷ǿ�
    if (cmdline.empty())
    {
        return false;
    }

    //�ڶ��������������У�����,�ŷָ��ַ��������õȺŷָ�
    std::vector<std::string> vecPara;
    split(cmdline, vecPara);

    typedef std::pair<std::string, std::string> CommandValuePair;
    std::vector<CommandValuePair> vecKey2Data;

    for (unsigned int i = 0; i < vecPara.size(); ++i)
    {
        std::vector<std::string> vecTmp;
        split(vecPara[i], vecTmp, "=");
        if (vecTmp.size() == 2)
        {
            vecKey2Data.push_back(CommandValuePair(vecTmp[0], vecTmp[1]));
        }
    }
    //--�����в����������

    //--��ʼ�жϲ����Ƿ���ָ����ʽ��
    bool bHaveID = false;
    bool bHaveKey = false;
    bool bHaveProcPara = false;

    for (unsigned int i = 0; i < vecKey2Data.size(); ++i)
    {
        if (vecKey2Data[i].first.compare("ID") == 0 && !vecKey2Data[i].second.empty())
        {
            bHaveID = true;
            m_strOpenId = vecKey2Data[i].second;
            continue;
        }

        if (vecKey2Data[i].first.compare("Key") == 0 && !vecKey2Data[i].second.empty())
        {
            bHaveKey = true;
            m_strOpenKey = vecKey2Data[i].second;
            continue;
        }

        if (vecKey2Data[i].first.compare("PROCPARA") == 0 && !vecKey2Data[i].second.empty())
        {
            bHaveProcPara = true;
            m_strProcPara = vecKey2Data[i].second;
            continue;
        }
    }

    if (!bHaveID || !bHaveKey || !bHaveProcPara)
    {
        clearArgs();
        return false;
    }

    //--����QQ�����ܵ�
    if (ConncetPipe())
    {
        return true;
    }
    else
    {
        clearArgs();
        return false;
    }
#else
    return false;
#endif
}

bool CQQHallManager::ConncetPipe()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    if (!LoadQQLibrary())
    {
        return false;
    }

    std::string strShowMsg = "Begin connect ProcPara:";
    strShowMsg += m_strProcPara;
    CCLOG(strShowMsg.c_str());

    //��������
    m_pProcMsgObj = CreateObj();
    if (m_pProcMsgObj == NULL)
    {
        return false;
    }

    m_pProcMsgObj->Initialize();
    m_pProcMsgObj->AddEventHandler(this);
    bool bSucc = m_pProcMsgObj->Connect(m_strProcPara.c_str());
    if (!bSucc)
    {
        CCLOG("ConnectFailed");
        /*
        ����ʧ�ܣ���ֱ���˳���Ϸ��δ������ʱ����������Ϊ��Ϸδ������������ֱ���˳�
        MessageBoxֻ��Ϊ��Demo��ʾ�����浯�����������˳��ͺ������˳������뿪���̸����Լ������ʵ��
        */
        //::MessageBox(NULL, "�ܵ�����ʧ�ܣ������˳�!", "�˳���ʾ", MB_OK);
    }

    return bSucc;
#endif
    return false;
}

void CQQHallManager::OnConnectSucc(IClientProcMsgObject* pClientProcMsgObj)
{
    m_bIsQQHall = true;
    CCLOG("ConnectSucc");
}

void CQQHallManager::OnConnectFailed(IClientProcMsgObject* pClientProcMsgObj, unsigned long dwErrorCode)
{
    CCLOG("ConnectFailed");
}

void CQQHallManager::OnConnectionDestroyed(IClientProcMsgObject* pClientProcMsgObj)
{
    CCLOG("ConnectionDestroyed");
}

void CQQHallManager::OnReceiveMsg(IClientProcMsgObject* pClientProcMsgObj, long lRecvLen, const char* pRecvBuf)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    PROCMSG_DATA stProcMsgData = {0};

    int nTotalLen = 0;
    int nCopyLen = 0;

    nCopyLen = sizeof(stProcMsgData.nCommandID);
    memcpy_s(&stProcMsgData.nCommandID, nCopyLen, pRecvBuf, nCopyLen);
    nTotalLen += nCopyLen;

    nCopyLen = sizeof(stProcMsgData.nDataLen);
    memcpy_s(&stProcMsgData.nDataLen, nCopyLen, pRecvBuf + nTotalLen, nCopyLen);
    nTotalLen += nCopyLen;

    nCopyLen = stProcMsgData.nDataLen;
    memcpy_s(stProcMsgData.abyData, MAX_PROCMSG_DATABUF_LEN, pRecvBuf + nTotalLen, nCopyLen);

    switch(stProcMsgData.nCommandID)
    {
    case SC_BOSSKEY:
        {
            std::string strMsg;
            int nCmd = *(int*)stProcMsgData.abyData;
            if (nCmd)
            {
                strMsg = "Receive msg SC_BOSSKEY(1 show��0 hide):1"; 
            }
            else
            {
                strMsg = "Receive msg SC_BOSSKEY(1 show��0 hide):0";
                cocos2d::Director::getInstance()->end();
            }

            CCLOG(strMsg.c_str());
        }   
        break;
    case SC_WND_BRINGTOP:
        {
            CCLOG("Receive msg SC_WND_BRINGTOP\n");
        }
        break;
    case SC_HALL_CMDPARA:
        {
            std::string strMsg = "Receive msg SC_HALL_CMDPARA ��";
            strMsg += (char*)stProcMsgData.abyData;
            CCLOG(strMsg.c_str());
        }
        break;
    case SC_RESPONSE_NEWCONN:
        {

            CCLOG("Receive msg SC_RESPONSE_NEWCONN \n");
        }
        break;
    case SC_RESPONSE_NEWCONN_RUFUSE:
        {
            CCLOG("Receive msg  SC_RESPONSE_NEWCONN_RUFUSE���ܾ�������");
        }
        break;
    default:
        break;
    }
#endif
}

void CQQHallManager::waitForQQPay()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	struct PayResult{
		int result;
		char bill[64];
	};

	HANDLE hpipe;
	hpipe = CreateNamedPipe(L"\\\\.\\pipe\\PSummonerPay",
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
		0,
		PIPE_UNLIMITED_INSTANCES,
		1024,
		1024,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	if (INVALID_HANDLE_VALUE == hpipe) {
		CCLOG("create PSummonerPay pipe error");
		return;
	}

	HANDLE hevent;
	hevent = ::CreateEvent(NULL, true, false, NULL); 

	OVERLAPPED  overlap;
	memset(&overlap, 0, sizeof(OVERLAPPED));
	overlap.hEvent = hevent;

	if (!ConnectNamedPipe(hpipe, &overlap)) {
		if (ERROR_IO_PENDING != ::GetLastError()) {
			CCLOG("connect PSummonerPay pipe error");
			return;
		}
	}
	// ���ͻ�������ʱ���¼���Ϊ���ź�  
	if (WAIT_FAILED == WaitForSingleObject(hevent, INFINITE)) {
		CCLOG("wait PSummonerPay pipe error");
		return;
	}
	// ������Ϣ
	PayResult pr;
	DWORD nobr = 0;
	memset(&pr, 0, sizeof(pr));
	ReadFile(hpipe, &pr, sizeof(pr), &nobr, NULL);
	// ���ѳɹ�
	if (pr.result != 0) {
		onLuaEventWithParamStr(41, pr.bill);
	}
	else {
		onLuaEvent(42);
	}
	
	::CloseHandle(hevent);
	::CloseHandle(hpipe);
#endif
}

void CQQHallManager::clearArgs()
{
    m_strOpenId = "";
    m_strOpenKey = "";
    m_strProcPara = "";
}

bool CQQHallManager::LoadQQLibrary()
{
    //ȡ������ĵ�ǰ·��
    //char szPath[MAX_PATH] = { 0 };
    //HMODULE hModule = ::GetModuleHandle(NULL);
    //GetModuleFileName(hModule, (LPWSTR)szPath, MAX_PATH);
    //char* pTemp = strrchr(szPath, '\\');
    //if (pTemp != NULL)
    //{
    //    pTemp[0] = '\0';
    //}

    //��dll·����ɾ���·��
    //std::string strDllPath = szPath;
    //strDllPath += szDllPath;
    //m_hModule = ::LoadLibrary((LPWSTR)strDllPath.c_str());

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    std::string strDllPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(szDllPath);
    std::wstring wstr = StringUtf8ToWideChar(strDllPath);
    m_hModule = ::LoadLibrary(wstr.c_str());

    if (m_hModule)
    {
        return true;
    }
    else
    {
        return false;
    }
#else
    return false;
#endif
}

IClientProcMsgObject* CQQHallManager::CreateObj()
{
    IClientProcMsgObject* pServerObj = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    CreateObjFunc pCreateObjFunc = NULL;
    pCreateObjFunc = (CreateObjFunc)::GetProcAddress(m_hModule, szCreateClientObjFunc);

    if (NULL == pCreateObjFunc)
    {
        return pServerObj;
    }

    pServerObj = pCreateObjFunc();
#endif
    return pServerObj;
}

void CQQHallManager::ReleaseObj(IClientProcMsgObject* pObj)
{
    ReleaseObjFunc pReleaseObjFunc = NULL;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    pReleaseObjFunc = (ReleaseObjFunc)::GetProcAddress(m_hModule, szReleaseClientObjFunc);

    if (NULL == pReleaseObjFunc)
    {
        return;
    }

    pReleaseObjFunc(pObj);
#endif
    return;
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
void CQQHallManager::SendMsgToGame(IClientProcMsgObject* pProcMsgObj, PROCMSG_DATA* pProcMsgData)
{
    if (NULL == pProcMsgData || NULL == pProcMsgObj)
    {
        return;
    }
    if (!pProcMsgObj->IsConnected())
    {
        CCLOG("pipe is not connected");
    }

    int nBufLen = pProcMsgData->nDataLen + sizeof(pProcMsgData->nCommandID) + sizeof(pProcMsgData->nDataLen);
    unsigned long dwRet = pProcMsgObj->SendMsg(nBufLen, (const char*)pProcMsgData);

    if (dwRet)
    {
        std::string strMsg = "SendMsgtoGame error :";
        char szErr[10] = { 0 };
        itoa(dwRet, szErr, 10);
        strMsg += szErr;
        CCLOG(strMsg.c_str());
    }
}
#endif
