/*
*   ��־��¼��
*   ��ΪKxLogger�Լ�IKxLogHandler������
*   KxLogger������־�Ĺ��ˣ����ݿ����Լ����ã���������IKxLogHandler
*   IKxLogHandler����ʵ����־��Ϣ�Ĵ���������д�ļ������������̨������������ʾ��
*
*   2015-04-24 By ��ү
*/
#ifndef __KX_LOG_H__
#define __KX_LOG_H__

#include <stdio.h>
#include <map>
#include <string>
#include <iostream>

#if KX_TARGET_PLATFORM == KX_PLATFORM_LINUX
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include <stdarg.h>
#endif

// ��־����
enum KXLOG_LEVEL
{
    KXLOG_INFO,         // ��Ϣ����
    KXLOG_DEBUG,        // ���Լ���
    KXLOG_ERROR         // ���󼶱�
};

// ��־����������
// ʵ����־�Ĵ���
class IKxLogHandler
{
public:
    IKxLogHandler()
        :m_Tag(1)
    { }
    virtual ~IKxLogHandler() { }
    // �����¼��־ʱ�ص���������־��Ϣ
    virtual bool onLog(int level, const std::string& log) = 0;
    // ����ӽ�KxLoggerʱ�ص�
    virtual void onEnter() { }
    // ��KxLogger���Ƴ�ʱ�ص�
    virtual void onExit() { }
    // ���������־ʱ�ص�
    virtual void onClean() { }

    inline int getHandleId()
    {
        return m_HandlerId;
    }

    inline void setHandleId(int handleId)
    {
        m_HandlerId = handleId;
    }

    int getTag() 
    { 
        return m_Tag; 
    }

    inline void setTag(int tag)
    {
        m_Tag = tag;
    }

protected:
    int m_HandlerId;
    int m_Tag;
};

// ��־���󣨵�����
// ������־�Լ�IKxLogHandler���
class KxLogger
{
private:
    KxLogger();
    virtual ~KxLogger();

public:
    static KxLogger* getInstance();
    static void destroy();

    // ���һ����־������
    bool addHandler(int handleId, IKxLogHandler* handler);
    // ����handler�����handleId���һ����־������
    bool addHandler(IKxLogHandler* handler);
    // �Ƴ�һ����־������
    bool removeHandler(int handleId);
    // ����handleId����ȡһ����־������
    IKxLogHandler* getHandler(int id);
    // ������е���־������
    void clearHandler();

    // д��һ����־��������־���������д���
    void writelog(int tag, int level, const char* file, const char* func, int line, const char* log, ...);
    // �����־��ֻ�ǵ�����־��������onClean����
    void clearLog();

    // ������־���Լ���
    inline void setLogLevel(int lv) { m_LogLevel = lv; };
    inline int getLogLevel() { return m_LogLevel; }

    // �������ȡ��־����ʾ��Ϣ
    inline bool isShowLevel() { return m_LevelOption; }
    inline bool isShowTime() { return m_TimeOption; }
    inline bool isShowDate() { return m_DateOption; }
    inline bool isShowFile() { return m_FileOption; }
    inline bool isShowFunc() { return m_FuncOption; }
    inline bool isShowLine() { return m_LineOption; }
    inline void setShowLevel(bool b) { m_LevelOption = b; }
    inline void setShowTime(bool b) { m_TimeOption = b; }
    inline void setShowDate(bool b) { m_DateOption = b; }
    inline void setShowFile(bool b) { m_FileOption = b; }
    inline void setShowFunc(bool b) { m_FuncOption = b; }
    inline void setShowLine(bool b) { m_LineOption = b; }

private:
    void updateFormat(int level, const char* log, const char* file, const char* func, int line);

private:
    int m_LogLevel;
    bool m_LevelOption;
    bool m_TimeOption;
    bool m_DateOption;
    bool m_FileOption;
    bool m_FuncOption;
    bool m_LineOption;
    std::string m_StringFormat;
    std::map<int, IKxLogHandler*> m_Handlers;
    static KxLogger* m_Instance;
};

#define KXLOG(tag, level, log, ...) KxLogger::getInstance()->writelog(tag, level, __FILE__, __FUNCTION__, __LINE__, log, ##__VA_ARGS__)
#define KXLOGDEBUG(log, ...) KxLogger::getInstance()->writelog(1, KXLOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, log, ##__VA_ARGS__)
#define KXLOGINFO(log, ...) KxLogger::getInstance()->writelog(1, KXLOG_INFO, __FILE__, __FUNCTION__, __LINE__, log, ##__VA_ARGS__)
#define KXLOGERROR(log, ...) KxLogger::getInstance()->writelog(1, KXLOG_ERROR, __FILE__, __FUNCTION__, __LINE__, log, ##__VA_ARGS__)
#define KXLOGBATTLE(...)       do {} while (0)
//#define KXLOGBATTLE(log, ...) KxLogger::getInstance()->writelog(1<<1, KXLOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, log, ##__VA_ARGS__)

#endif
