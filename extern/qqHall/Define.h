#pragma once

#define CS_GAME_EXIT        0x00000001  // �ر���Ϸ�ͻ���
#define CS_NAVIGATE_URL     0x00000003  // ��Ϸ�����������½̬��һ����ǶIEҳ

//--�������ͻ���
#define CS_REQUEST_UIN		0x00000004
#define CS_Notify_APP_TOP   0x00000008   //APP����Ϣ����֪ͨ�˸�app�ö�
#define CS_Notify_APP_CLOSE 0x00000009   //app����Ϣ����֪ͨ�û�����˹رգ����Ǻ�̨��������פ���������յ�����Ϣ����������ʱ����ͳ��
#define CS_REQUSET_VERSION  0x0000000A   //app����Ϣ��������汾��
//--end�������ͻ�����Ϸר��

#define CS_REQ_NEWCONNECTION 0x0000000B  //app����Ϣ�������������
#define CS_REQ_NEWACCOUNT  0x0000000C    //app����Ϣ���������ʺŵ�¼

//������������>��Ϸ
#define SC_PLAYERINFO       0x00000001  // ����Ϸ�·��Լ�����Ϣ
#define SC_NOTIFY_KEY       0x00000002  // ֪ͨ�ͻ���st�仯��Ҳ��request�Ļظ�
#define SC_BOSSKEY          0x00000003  // ֪ͨ��Ϸ�ϰ����0��hide 1��show
#define SC_WND_BRINGTOP     0x00000004  // ֪ͨ��Ϸ������ǰ��ʾ
#define SC_OPEN_KEY         0x00000005  // ����Ϸ�·�openID��openKey
#define SC_HALL_CMDPARA     0x00000006  // ����Ϸ�������������������Ϸ�������������ִ�ж�Ӧ�Ĺ��ܣ��ò�������Ϸ������
#define SC_RESPONSE_UIN		0x00000007  // ͬCS_REQUEST_UIN��Ӧ
#define SC_RESPONSE_NEWCONN      0x0000000B  //�����ظ���Ϸ����������
#define SC_RESPONSE_NEWCONN_RUFUSE  0x0000000C  //�����ظ���Ϸ���������Ӵ���
//------------------------------------------------------------------------------

#define MAX_PROC_START_CMD_SIZE 128 // �������̲�������
#define MAX_CONNECTION_NAME_SIZE 128
#define  MAX_PROCMSG_DATABUF_LEN 64*1024

#pragma pack(1)
typedef struct stProcMsgData
{
    int  nCommandID;    //Э��ID
    int  nDataLen;      //Э�����ݳ���
    char abyData[MAX_PROCMSG_DATABUF_LEN];
}PROCMSG_DATA;
#pragma pack()
