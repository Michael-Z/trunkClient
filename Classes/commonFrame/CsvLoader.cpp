#include "CsvLoader.h"
#include "CommTools.h"

#ifndef RunningInServer

#include "cocos2d.h"
#include "xxtea/xxtea.h"
USING_NS_CC;

#endif

using namespace std;

CCsvLoader::CCsvLoader()
{
    m_nDocOffset = 0;
    m_nDocStep = 0;
    m_nLineOffset = 0;
    m_nLineStep = 0;
}


CCsvLoader::~CCsvLoader()
{
}

void CCsvLoader::Release()
{
	m_nDocOffset = 0;
	m_nDocStep = 0;
	m_nLineOffset = 0;
	m_nLineStep = 0;
	m_CurLine = "";
	m_Document = "";
}

bool CCsvLoader::LoadCSV(const char* fileName)
{
    if (NULL == fileName)
    {
        return false;
    }

#ifndef RunningInServer

	auto fileUtils = FileUtils::getInstance();
	if (!fileUtils->isFileExist(fileName)) {
		string fname(fileName);
		size_t pos = fname.find_last_of('.');
		if (pos != string::npos) {
			fname.replace(pos, 4, ".csve");		// ".csv" -> ".csve"
		}
		else {
			fname += ".cvse";
		}
		auto encrypted = fileUtils->getDataFromFile(fname);
        if (!encrypted.isNull())
        {
            string key("fanhougame_zhs");
            xxtea_long len;
            auto decrypted = xxtea_decrypt((unsigned char *)encrypted.getBytes(), encrypted.getSize(), (unsigned char *)key.c_str(), key.length(), &len);
            m_Document = string((const char *)decrypted);
        }
	}
	else {
		m_Document = fileUtils->getStringFromFile(fileName);
	}

#else
    m_Document = getStringFromFile(fileName);
#endif

    m_nDocOffset = m_nDocStep = 0;
    
    //����ļ�Ϊ��Ҳ�᷵��ʧ��
    return NextLine();
}

// NextLine����ת����һ��
// ����false��ʾ�Ѿ�����β��������true��ʾ�������µ�һ������
bool CCsvLoader::NextLine()
{
skipline:
    // ����������Ҫ����\r\n������
    m_nDocStep = m_Document.find_first_of('\n', m_nDocOffset);

    if (-1 == m_nDocStep)
    {
        // ���ﻹ��Ҫ�������һ���Ҳ���\n�����
        if (m_nDocOffset < m_Document.size())
        {
            // �����ʣ���һ������ȡ��
            m_CurLine = m_Document.substr(m_nDocOffset, m_Document.size() - m_nDocOffset);
            m_nDocOffset = m_Document.size();
            m_nLineOffset = 0;
            if (isCurrentLineNull())
            {
                return false;
            }
            return true;
        }
        return false;
    }

    if (m_Document.at(m_nDocStep - 1) == '\r')
    {
        m_CurLine = m_Document.substr(m_nDocOffset, m_nDocStep - m_nDocOffset - 1);
    }
    else
    {
        m_CurLine = m_Document.substr(m_nDocOffset, m_nDocStep - m_nDocOffset);
    }
	m_nLineOffset = 0;
	m_nDocOffset = m_nDocStep + 1;

	if (isCurrentLineNull())
	{
		goto skipline;
	}
	else
	{
		return true;
	}
}

std::string CCsvLoader::LeftStr()
{
    return m_CurLine.substr(m_nLineOffset, m_CurLine.size() - m_nLineOffset);;
}

// ����ǰ����һ���ֶν���Ϊstring������
// �������ʧ�ܷ���""
std::string CCsvLoader::NextStr()
{
	m_nLineStep = m_CurLine.find_first_of(',', m_nLineOffset);
	string seg = m_CurLine.substr(m_nLineOffset, m_nLineStep - m_nLineOffset);
	m_nLineOffset = m_nLineStep + 1;
	return Trim(seg);
}

// ����ǰ����һ���ֶν���Ϊint������
// �������ʧ�ܷ���0
int CCsvLoader::NextInt()
{
	return atoi(NextStr().c_str());
}

// ����ǰ����һ���ֶν���Ϊfloat������
// �������ʧ�ܷ���0.0f
float CCsvLoader::NextFloat()
{
    return static_cast<float>(toolToNum<float>(NextStr().c_str()));
}

std::string CCsvLoader::Trim(std::string str)
{
	if (str.empty())
	{
		return str;
	}
	size_t posL = str.find_first_not_of(' ');
    size_t posR = str.find_last_not_of(' ');
    if ((posL == string::npos && posR == string::npos)
        || (0 == posL && (str.size()-1) == posR))
	{
		return str;
	}
	string ret = str.substr(posL, posR - posL + 1);
	return ret;
}

bool CCsvLoader::isCurrentLineNull()
{
	for (size_t i = 0; i < m_CurLine.length(); ++i)
	{
		switch (m_CurLine.c_str()[i])
		{
		case ' ':
		case ',':
		case '\n':
		case '\r':
			break;
			
		default:
			return false;
			break;
		}
	}
	return true;
}

// ��һ���ַ����Էָ��� sep �ָ�����ת��Ϊ�ַ���������vector������
bool CCsvLoader::SplitStrToVector(const std::string &str, char sep, std::vector<std::string>& out)
{
    int pos = 0;
    int step = 0;

    while (pos < static_cast<int>(str.length()) && step != -1)
    {
        step = str.find_first_of(sep, pos);
        string seg = str.substr(pos, step);
        out.push_back(seg);
        pos = step + 1;
    }

    return out.size() > 0;
}

// ��һ���ַ����Էָ��� sep �ָ�����ת��Ϊfloat������vector������
bool CCsvLoader::SplitFloatToVector(const std::string &str, char sep, std::vector<float>& out)
{
    int pos = 0;
    int step = 0;

    while (pos < static_cast<int>(str.length()) && step != -1)
    {
        step = str.find_first_of(sep, pos);
        string seg = str.substr(pos, step);
        float value = static_cast<float>(toolToNum<float>(seg.c_str()));
        out.push_back(value);
        pos = step + 1;
    }

    return out.size() > 0;
}

// ��һ���ַ����Էָ��� sep �ָ�����ת��Ϊint������vector������
bool CCsvLoader::SplitIntToVector(const std::string &str, char sep, std::vector<int>& out)
{
    int pos = 0;
    int step = 0;

    while (static_cast<unsigned int>(pos) < str.length() && step != -1)
    {
        step = str.find_first_of(sep, pos);
        string seg = str.substr(pos, step);
        int value = atoi(seg.c_str());
        out.push_back(value);
        pos = step + 1;
    }

    return out.size() > 0;
}

std::string CCsvLoader::getStringFromFile(const std::string &path)
{
    FILE *fp = fopen(path.c_str(), "rt");
    if (NULL == fp)
    {
        return "";
    }

    char* buffer = NULL;
    int size = 0;
    int readsize = 0;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    buffer = (char*)malloc(sizeof(char) * (size + 1));
    buffer[size] = '\0';

    readsize = fread(buffer, sizeof(unsigned char), size, fp);
    fclose(fp);

    std::string ret(buffer);
    return ret;
}
