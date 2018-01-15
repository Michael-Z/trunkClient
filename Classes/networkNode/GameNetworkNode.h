/*
*   ��������ĳ�ʼ��, ����״̬����, �Լ�������ѯ
*
*/

#ifndef __GAMENETWORKNODE_H__
#define __GAMENETWORKNODE_H__

#include <functional>
#include "cocos2d.h"
#include "KxServer.h"
#include "KxCSComm.h"

enum EServerConnType
{
    SERVER_CONN_SESSION,      // session����������
    SERVER_CONN_CHAT,         // chat����������
};

struct ServerConn
{
    std::string                 ConnIP;         // ���ӵ�ip��ַ
    int                         Port;               // ���ӵĶ˿�
    int                         ConnectingTimes;    // ���������еĴ���������10��ǿ�ƹر�������
    KxServer::KXSOCK_VERSION	SockVersion;        // ipv4/ipv6
    KxServer::KxTCPConnector*   Connector;          // ����
    std::function<void(bool)>   ConnectCallback;

    ServerConn() :Port(0)
        , ConnectingTimes(0)
        , SockVersion(KxServer::KXV_IPV6)
        , Connector(nullptr)
        , ConnectCallback(nullptr)
    {
    }
};

class CBaseModule;
class CGameNetworkNode : 
    public Node
{
private:
    CGameNetworkNode(void);
    ~CGameNetworkNode(void);

public:

    static CGameNetworkNode *getInstance();
    static void destroy();

    bool init();
    void onEnter();
    void onExit();
    void update(float dt);
    void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags);

    CREATE_FUNC(CGameNetworkNode);

    // ���ӵ�ָ��ip, port
    bool connectToServer(const char *ip, int port, EServerConnType connType, KxServer::KXSOCK_VERSION version = KxServer::KXV_IPV6,
        std::function<void(bool)> callBack = nullptr);
    // ��������
    int sendData(char* buffer, unsigned int len, EServerConnType connType = SERVER_CONN_SESSION);
    // �ṩ�����ӿ�, ����֮ǰѡ��ķ�����ip��ַ
    bool reconnectToServer(EServerConnType connType, std::function<void(bool)> callBack = nullptr);
    // �Ƿ���������, Ĭ������
    void setIsHeartbeat(bool bEnabled);
    // ��������ʱ��
    void setHeartbeatTime(float fHbTime);
    // �Ͽ�����
    void closeConnect();
    // �������
    KxServer::KxTCPConnector *getConnector(EServerConnType connType = SERVER_CONN_SESSION);
    // ��ȡ������Ϣ
    ServerConn* getServerConn(EServerConnType connType = SERVER_CONN_SESSION);
	// �Ƿ��û������ر�
    bool isUserClose(){ return m_bUserClose; }

private:
    // ����, �������״̬
    void heartbeatScheduler(float dt);
    // ע������ͨ����Ϣ, ��������������ʱ��
    void registeNetworkCommunicate(void *data);

private:
    bool                                m_bUserClose;
    bool                                m_bEnabledHb;                   // �Ƿ���������
    float                               m_fMaxHbTime;                   // ���û���ݽ�������һ��������
    float                               m_fTickNet;                     // �������ϴν������ݼ��ʱ��
    static CGameNetworkNode*            m_pInstance;                    // 
    KxServer::IKxCommPoller*            m_pPoller;                      // ��ѯ��
    CBaseModule*                        m_pGameModule;                  // �����߼�ģ��ʵ��
    CBaseModule*                        m_pChatModule;                  // ��������ģ��ʵ��

    std::map<int, ServerConn>           m_mapServerConns;               // ����������
};

#endif 
