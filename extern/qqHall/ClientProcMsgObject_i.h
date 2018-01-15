#pragma once

class IClientProcMsgObject;
class IClientProcMsgEventHandler;

class IClientProcMsgObject
{
public:
    virtual bool Initialize() = 0;

    //--------------------------------------------------------------------------
    // Method:    BuildConnection
    // Returns:   BOOL�����������Ƿ�ɹ�
    // Parameter: lpszConnectionName ���ӵı�ʶ��
    // Brief:     ���ӽ��̽������ӣ�����ͨ�ŷ�ʽ�ڲ�lpszConnectionName���ж�
    //--------------------------------------------------------------------------
    virtual bool Connect(const char* lpszConnectionName) = 0;

    //--------------------------------------------------------------------------
    // Method:    Disconnect
    // Returns:   void
    // Parameter: void
    // Brief:     �Ͽ�����
    //--------------------------------------------------------------------------
    virtual void Disconnect() = 0;

    //--------------------------------------------------------------------------
    // Method:    IsConnected
    // Returns:   BOOL���ж����ӻ���Ч
    // Parameter: void
    // Brief:     �ж����ӻ���Ч
    //--------------------------------------------------------------------------
    virtual bool IsConnected() = 0;

    //--------------------------------------------------------------------------
    // Method:    SendMessage
    // Returns:   0, �������ݳɹ�, ��0������ʧ��
    // Parameter: @pbySendBuf  ���͵�����
    // Brief:     ��������
    //--------------------------------------------------------------------------
    virtual unsigned long SendMsg(long lLen, const char* pbySendBuf) = 0;

    //--------------------------------------------------------------------------
    // Method:    AddEventHandler
    // Returns:   void
    // Parameter: pEventHandler���������ͨ���¼��ӿ�
    // Brief:     ��ӽ���ͨ�ż���
    //--------------------------------------------------------------------------
    virtual void AddEventHandler(IClientProcMsgEventHandler* pEventHandler) = 0;

    //--------------------------------------------------------------------------
    // Method:    RemoveEventHandler
    // Returns:   void
    // Parameter: pEventHandler���������ͨ���¼��ӿ�
    // Brief:     ȡ������ͨ�ż���
    //--------------------------------------------------------------------------
    virtual void RemoveEventHandler(IClientProcMsgEventHandler* pEventHandler) = 0;
};

class IClientProcMsgEventHandler
{
public:
    //--------------------------------------------------------------------------
    // Method:    OnBuildConnectionSucc
    // Returns:   void
    // Parameter: void
    // Brief:     �������ӳɹ�
    //--------------------------------------------------------------------------
    virtual void OnConnectSucc(IClientProcMsgObject* pClientProcMsgObj) = 0;

    //--------------------------------------------------------------------------
    // Method:    OnBuildConnectionFailed
    // Returns:   void
    // Parameter: dwErrorCode��ʧ�ܵĴ�����
    // Brief:     ��������ʧ��
    //--------------------------------------------------------------------------
    virtual void OnConnectFailed(IClientProcMsgObject* pClientProcMsgObj
        , unsigned long dwErrorCode) = 0;

    //--------------------------------------------------------------------------
    // Method:    OnConnectionDestroy
    // Returns:   void
    // Parameter: void
    // Brief:     ���ӱ��ƻ����Ͽ�
    //--------------------------------------------------------------------------
    virtual void OnConnectionDestroyed(IClientProcMsgObject* pClientProcMsgObj) = 0;

    //--------------------------------------------------------------------------
    // Method:    OnReceiveMsg
    // Returns:   void
    // Parameter: pProcMsgData �յ�������
    // Brief:     �յ���һ���̷���������
    //--------------------------------------------------------------------------
    virtual void OnReceiveMsg(IClientProcMsgObject* pClientProcMsgObj
        , long lRecvLen, const char* pRecvBuf) = 0;
};