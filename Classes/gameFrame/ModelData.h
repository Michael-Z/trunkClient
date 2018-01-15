#ifndef _SUM_MODEL_DATA_
#define _SUM_MODEL_DATA_

#include <map>
#include <set>
#include <vector>
#include <string>
#include "LoginProtocol.h"
#include "ConfStage.h"
#include "ConfGameSetting.h"

enum ETalentStatus
{
    ETS_LOCK,            // δ����
    ETS_UNLOCK,          // ����
    ETS_UNACTIVE,        // δ����
    ETS_ACTIVE,           // ����
    //ETS_EQUIPACTIVE,      // װ������(ǰ��)
};

//�½�״̬
enum EChapterState
{
	ECS_LOCK,			//δ����
	ECS_UNLOCK,			//�ѽ���
	ECS_FINISH,			//�����
	ECS_REWARD,			//����ȡ
};

//�ؿ�״̬
enum ELevelState
{
	ESS_HIDE,			//δ��ʾ
	ESS_LOCK,			//δ����
	ESS_UNLOCK,			//�ѽ���
	ESS_ONE,			//һ��
	ESS_TWO,			//����
	ESS_TRI,			//����
};

// װ����������
enum EquipPartType
{
    WEAPON = 1,					// ����
    HEADWEAR,					// ͷ��
    CLOTH,						// �·�
    SHOES,						// Ь��
    ACCESSORY,					// ��Ʒ
    TREASURE,					// ����
};


/////////////////////////////////////////////////////////////////////////�û�ģ��
class CUserModel
{
public:
	CUserModel();

public:

	bool init(void *data);

	void setUserID(int id){ m_nUserID = id; }
	int	getUserID(){ return m_nUserID; }

	void setHeadID(int id){ m_nHeadID = id; }
	int	getHeadID(){ return m_nHeadID; }

	void setGold(int num){ m_nGold = num; }
    void addGold(int num){ m_nGold += num; }
	int	getGold(){ return m_nGold; }

	void setLevel(int lv){ m_nLevel = lv; }
	int	getLevel(){ return m_nLevel; }

    void setUserExp(int exp) { m_nUserExp = exp; }
	int	getUserExp(){ return m_nUserExp; }

	void setDiamond(int num) { m_nDiamond = num; }
	int	getDiamond(){ return m_nDiamond; }

	void setTowerCoin(int coin) { m_nTowerCoin = coin; }
	int	getTowerCoin() { return m_nTowerCoin; }

	void setPVPCoin(int coin) { m_nPvpCoin = coin; }
	int	getPVPCoin() { return m_nPvpCoin; }

	void setUnionContrib(int contrib) { m_nUnionContrib = contrib; }	
	int	getUnionContrib() { return m_nUnionContrib; }

	void setFlashcard(int flashcard) { m_nFlashcard = flashcard; }
	int	getFlashcard(){ return m_nFlashcard; }

	void setFlashcard10(int flashcard10) { m_nFlashcard10 = flashcard10; }
	int	getFlashcard10(){ return m_nFlashcard10; }

	void setMaxEnergy(int max){ m_nMaxEnergy = max; }
	int	getMaxEnergy(){ return m_nMaxEnergy; }

    void setVipLv(int lv){ m_nVipLv = lv; }
	int getVipLv(){ return m_nVipLv; }

    void setVipPayment(int payment){ m_nPayment = payment; }
    void addVipPayment(int payment){ m_nPayment += payment; }
    int getVipPayment(){ return m_nPayment; }

    void setVipScore(int score){ m_nVipScore = score; }
    void addVipScore(int score){ m_nVipScore += score; }
    int getVipScore(){ return m_nVipScore; }

    void setMonthCardStamp(int time){ m_nMonthCardStamp = time; }
    int getMonthCardStamp(){ return m_nMonthCardStamp; }

    void setBuyGoldTimes(int times){ m_nBuyGoldTimes = times; }
	int getBuyGoldTimes(){ return m_nBuyGoldTimes; }

	const char* getUserName(){ return m_cUserName; }
    void setUserName(const char* name);
    void setChangeNameFree(){ m_nChangeNameFree = 1; }

	void setFreeHeroTimes(int times) { m_nFreeHeroTimes = times; }
	int	getFreeHeroTimes() { return m_nFreeHeroTimes; }
    
	int getChangeNameFree() { return m_nChangeNameFree; }
	    
    int getTotalSignDay(){ return m_nTotalSignDay; }
    void setTotalSignDay(int totalSignDay){ m_nTotalSignDay = totalSignDay; }
    
    int getMonthSignDay(){ return m_nMonthSignDay; }
    void setMonthSignDay(int monthSignDay){ m_nMonthSignDay = monthSignDay; }
    
    int getTotalSignFlag(){ return m_nTotalSignFlag; }
    void setTotalSignFlag(int totalSignFlag){ m_nTotalSignFlag = totalSignFlag; }

    // ��������
    void resetUserData();

protected:

	int			m_nUserID;					//�û�ID
	int			m_nHeadID;					//ͷ��ID
	int			m_nGold;					//���
	int			m_nLevel;					//�ȼ�
    int         m_nUserExp;					//����
	int			m_nDiamond;					//��ʯ
	int			m_nTowerCoin;				//����
	int			m_nPvpCoin;					//pvp��
	int			m_nUnionContrib;			//���ṱ��
	int			m_nFlashcard;				//�鿨ȯ
	int			m_nFlashcard10;				//ʮ���鿨ȯ
	int			m_nMaxEnergy;				//�������
    int         m_nVipLv;					//VIP
    int         m_nPayment;              //��֧�������
    int         m_nVipScore;                //vip����
    int         m_nMonthCardStamp;          // �¿�����ʱ���
    int         m_nChangeNameFree;			//�Ƿ���Ѹ���,0���1�����
    int         m_nBuyGoldTimes;			//�����Ҵ���
    int			m_nFreeHeroTimes;			//�ϴ����Ӣ��ʱ��
    int         m_nTotalSignDay;            //�ۼ�ǩ������
    int         m_nMonthSignDay;            //�����ۼ�ǩ������
    int         m_nTotalSignFlag;           //�ۼ�ǩ������

	char		m_cUserName[32];			//�û���
};

/////////////////////////////////////////////////////////////////////////����ģ��
class CBagModel
{
public:
	CBagModel();

public:
	bool		init(void *data);
	//��չ�����޸���������
	bool		extra(int add);
	//�����Ʒ:װ��ID������ID������ƷID������
	bool		addItem(int id, int param);
	//ɾ����Ʒ
	bool		removeItem(int id);
    //ɾ����Ʒ
    bool		removeItems(int id, int count = 1);
	//�ж��Ƿ���ĳ��Ʒ
	bool		hasItem(int id);
    //��ȡ������ŵ�����
    int         getItemCount() { return (int)m_mapBagItems.size(); }

public:
	//��ȡ��Ʒ�б�����Ʒ
	std::map<int, int>& getItems(){ return m_mapBagItems; }

	//��ȡ��ǰ����
	int         getCurCapacity(){ return m_nCurCapacity; }
	void        setCurCapacity(int cur){ m_nCurCapacity = cur; }

protected:
	int			m_nCurCapacity;			//��ǰ����
	std::map<int, int>	m_mapBagItems;	//������Ʒ�б�
};

/////////////////////////////////////////////////////////////////////////װ��ģ��
class CEquipModel
{
public:
	//��ʼ��
	bool init(void *data);
	//�Ƿ���װ��
	bool haveEquip(int equipId);
	//���װ��
	bool addEquip(EquipItemInfo &EquipData);
	//�Ƴ�װ��
	void removeEquip(int equipId);
	//���װ��������id
	int getEquipConfId(int equipId);
    //��ȡװ������
    bool getEquipInfo(int equipId, EquipItemInfo &info);
	//�������װ��
	std::map<int, EquipItemInfo> &getEquips() { return m_mapEquips; }

protected:
	std::map<int,EquipItemInfo> m_mapEquips;		//װ���б�
};

/////////////////////////////////////////////////////////////////////////�ٻ�ʦ�б�ģ��
class CSummonersModel
{
public:
	//��ʼ������
	bool		init(void *data);
	//�����ٻ�ʦ
	bool		addSummoner(int id);
	//���ٻ�ʦô
	bool		hasSummoner(int id);
	//��ȡ�ٻ�ʦ
	int			getSummonerCount();

public:
	//��ȡ�ٻ�ʦ�б�
	std::vector<int>&	getSummoners(){ return m_vecSummoners; }

protected:
	std::vector<int>	m_vecSummoners;		//�ٻ�ʦ�б�
};

/////////////////////////////////////////////////////////////////////////Ӣ�ۿ�Ƭģ��
class CHeroCardModel
{
public:
    CHeroCardModel();
    bool init(void* data);

public:
    void setID(int id)      { m_nID = id;} 
    void setFrag(int frag)  { m_nFrag = frag; }
    void setLevel(int lv)   { m_nLv = lv; }
    void setStar(int star)  { m_nStar = star; }
    void setExp(int exp)    { m_nExp = exp; }

    // ���, ɾ��, �滻 Ӣ��װ�� (eqDynIDΪ0ʱ��ʾ�ò�λû��װ��)
    void setEquip(EquipPartType eqPart, int eqDynID){
        if (eqPart > 0 && eqPart <= 6)
            m_equips[eqPart - 1] = eqDynID;
    }

    int getID()         { return m_nID; }
    int getFrag()       { return m_nFrag; }
    int getLevel()      { return m_nLv; }
    int getStar()       { return m_nStar; }
    int getExp()        { return m_nExp; }

    int getEquip(EquipPartType eqPart){
        if (eqPart > 0 && eqPart <= 6)
            return m_equips[eqPart - 1];
        return 0;
    }
    int* getEquips() { return m_equips; }

private:
    int m_nID;		               // Ӣ��ID
    int m_nFrag;                   // Ӣ����Ƭ
    int m_nLv;                     // �ȼ�
    int m_nStar;                   // �Ǽ�
    int m_nExp;                    // ����
    unsigned char m_talent[8];     // �츳
    int m_equips[6];               // 6��װ��
};

/////////////////////////////////////////////////////////////////////////Ӣ�ۿ���ģ��
class CHeroCardBagModel
{
public:
public:
    bool init(void *data);
    bool addHeroCard(int id);
    bool hasHeroCard(int id);
    CHeroCardModel* getHeroCard(int id);
    std::map<int, CHeroCardModel*>& getHeroCards(){ return m_mapHeroCards; }
    int getHeroCardCount();
private:
    std::map<int, CHeroCardModel*>	m_mapHeroCards;		//Ӣ�ۿ�Ƭ
};


/////////////////////////////////////////////////////////////////////////�ؿ�ģ��
class CStageModel
{
public:
	bool init(void *data);
	std::map<int, int>& getChapterStates(){ return m_mapChapterStates; }
    std::map<int, int>& getComonStageStates(){ return m_mapComonStageStates; }
    std::map<int, int>& getEliteStageStates(){ return m_mapEliteStageStates; }

public:
	int getCurrentComonStageID();							//��ȡ��ǰ��ͨ�ؿ�
	int getCurrentEliteStageID();						    //��ȡ��ǰ��Ӣ�ؿ�
	int getChapterState(int ch);							//��ȡ�½�״̬
    int getComonStageState(int lv);							//��ȡ��ͨ�½�״̬
    int getEliteStageState(int lv);							//��ȡ��Ӣ�ؿ�״̬
	int getEliteChallengeCount(int lv);						//��ȡ��Ӣ�ؿ��Ѿ���ս�Ĵ���
	int getEliteChallengeTimestamp(int lv);					//��ȡ��Ӣ��սʱ���
	int getEliteBuyCount(int lv);							//��ȡ���عؿ��������
	int getEliteBuyTimestamp(int lv);						//��ȡ��Ӣ����ʱ���

	void setCurrentComonStageID(int lv);					//���õ�ǰ�նȹؿ�
	void setCurrentEliteStageID(int lv);					//���õ�ǰ��Ӣ�ؿ�
	void setChapterState(int ch, int state);				//������ͨ�½�״̬
	void setComonStageState(int lv, int state);				//������ͨ�½�״̬
	void setEliteStageState(int lv, int state);				//���þ�Ӣ�ؿ�״̬
	void setEliteChallengeCount(int lv, int count);			//���þ�Ӣ�ؿ��Ѿ���ս�Ĵ���
	void setEliteChallengeTimestamp(int lv, int time);		//���þ�Ӣ��սʱ���
	void setEliteBuyCount(int lv, int count);				//���þ�Ӣ�ؿ��������
	void setEliteBuyTimestamp(int lv, int time);			//���þ�Ӣ����ʱ���

public:
	void resetEliteChallengeCount(int lv);					//���þ�Ӣ�ؿ���ս����
	void resetEliteBuyCount(int lv);						//���þ�Ӣ�ؿ��������						

protected:
	int deltaDay(const TimeInfo& info);						//�ռ��
	int deltaWeek(const TimeInfo& info);					//�ܼ��
	int getChapterIDByStageID(int id);						//��ȡ��ǰ�½�ID

protected:
	int	m_nCurrentComonStageID;								//��ǰ��ͨ�ؿ�
	int m_nCurrentEliteStageID;								//��ǰ��Ӣ�ؿ�
	std::map<int, int> m_mapChapterStates;					//��ͨ�½�״̬
	std::map<int, int> m_mapComonStageStates;				//��ͨ�ؿ�״̬
	std::map<int, int> m_mapEliteStageStates;				//��Ӣ�ؿ�״̬
	std::map<int, int> m_mapEliteStageChallengeCount;		//��Ӣ�ؿ��Ѿ���ս�Ĵ���
	std::map<int, int> m_mapEliteStageChallengeTimestamp;	//��Ӣ�ؿ���սʱ���
	std::map<int, int> m_mapEliteStageBuyCount;				//��Ӣ�ؿ��������
	std::map<int, int> m_mapEliteStageBuyTimestamp;			//��Ӣ�ؿ�����ʱ���
};

enum ETeamType
{
    ETT_PASE,        // ͨ�ض���
    ETT_SPORTE       // ��������
};

struct TeamInfo
{
    int teamType;                 // ��������
    int summonerID;               // �ٻ�ʦID
	int heroID[7];				  // Ӣ��id
};

/////////////////////////////////////////////////////////////////////////����ģ��
class CTeamModel
{
public:
    bool init(void *data);
    // ���ݶ������ͻ�ȡ����
    bool getTeamInfo(int teamType, int &summonerID, std::vector<int>& vecHero);
    // ���ݶ����������ö�����Ϣ
    void setTeamInfo(int teamType, int summonerID, const std::vector<int>& vecHero);
    // �����ж�����ȥ��ĳ��Ӣ��
    void removeHeroFromAllTeam(int heroID);
    // Ӣ���Ƿ���ڶ�����(�������)
    bool hasHeroAllTeam(int heroID);

private:
    std::map<int, int>              m_mapTeamSummoner;  // teamType, summoner
    std::map<int, std::vector<int>> m_mapTeamHero;      // teamType, herolist
};

// ����״̬(״̬������,��ǰ�˼���)
enum ETaskStatus
{
	 ETASK_UNATIVE = -1,          // δ����(�ȼ�δ��)
	 ETASK_ACTIVE,                // ����״̬
	 ETASK_FINISH,                // ���(����ȡ)
	 ETASK_GET,                   // �Ѿ���ȡ
};

struct EquipInfo
{
	 int equipID;            // װ��ΨһID
	 int equipConfID;        // װ������ID
};

struct ItemInfo
{
	 int itemID;             // ���ñ�ID
	 int itemCount;          // ��������
};

class CTaskModel
{
public:
	bool init(void* data);
	bool addTask(const TaskInfo& taskInfo);
	bool delTask(const int& taskId);
	bool setTask(const TaskInfo& taskInfo);
	inline const std::map<int, TaskInfo>& getTasksData() { return m_mapTasksInfo; }
private:
	std::map<int, TaskInfo> m_mapTasksInfo;
};

enum EAchieveStatus
{
	 EACHIEVE_STATUS_UNACTIVE = -1,// δ����
	 EACHIEVE_STATUS_ACTIVE,       // ����
	 EACHIEVE_STATUS_FINISH,       // ���
	 EACHIEVE_STATUS_GET,          // ��ȡ
};

class CAchieveModel
{
public:
	 bool init(void* data);
	 bool addAchieve(const AchieveInfo& achieveInfo);
	 bool delAchieve(const int& achieveID);
	 // ���óɾ�
	 bool setAchieve(const AchieveInfo& achieveInfo);
	 inline const std::map<int, AchieveInfo>& getAchievesData() { return m_mapAchievesInfo; }
private:
	std::map<int, AchieveInfo> m_mapAchievesInfo;
};

class CGuideModel
{
public:
	 bool init(void* data);

public: 
	void del(int id);
	void add(int id);
	std::set<int>& getActives(){ return m_setActives; }

private:
	 std::set<int> m_setActives;
};

class CUnionModel
{
public:
    CUnionModel();
    bool init(void* data);

    bool getHasUnion(){ return m_bHasUnion; }
    void setHasUnion(bool hasUnion){
        m_bHasUnion = hasUnion;
        if (hasUnion)
        {
            m_nApplyCount = 0;
            m_nApplyStamp = 0;
            m_vecApplyInfo.clear();
        }
        else
        {
            m_bHasAudit = false;
            m_nUnionID = 0;
            m_nTodayLiveness = 0;
            m_nTotalContribution = 0;
            m_cPos = 0;
            memset(m_cUnionName, 0, sizeof(m_cUnionName));
            memset(m_cUnionNotice, 0, sizeof(m_cUnionNotice));
        }
    }

    bool getHasAudit(){ return m_bHasAudit; }
    int getUnionID(){ return m_nUnionID; }
    int getTodayLiveness(){ return m_nTodayLiveness; }
    int getTotalContribution(){ return m_nTotalContribution; }
    char    getPos(){ return m_cPos; }
    const char* getUnionName(){ return m_cUnionName; }
    const char* getUnionNotice(){ return m_cUnionNotice; }

    void setHasAudit(bool hasAudit){ m_bHasAudit = hasAudit; }
    void setUnionID(int id){ m_nUnionID = id; }
    void setTodayLiveness(int value){ m_nTodayLiveness = value; }
    void setTotalContribution(int value){ m_nTotalContribution = value; }
    void setPos(int pos){ m_cPos = pos; }
    void setUnionName(const char* name)
    {
        if (strlen(name) < sizeof(m_cUnionName))
        {
            memset(m_cUnionName, 0, sizeof(m_cUnionName));
            memcpy(m_cUnionName, name, strlen(name));
        }
    }
    void setUnionNotice(const char* notic)
    {
        if (strlen(notic) < sizeof(m_cUnionName))
        {
            memset(m_cUnionNotice, 0, sizeof(m_cUnionNotice));
            memcpy(m_cUnionNotice, notic, strlen(notic));
        }
    }

    int getApplyCount(){ return m_nApplyCount; }
    int getApplyStamp(){ return m_nApplyStamp; }
    const std::vector<ApplyInfo> getApplyInfo(){ return m_vecApplyInfo; }

    void setApplyCount(int count){ m_nApplyCount = count; }
    void setApplyStamp(int stamp){ m_nApplyStamp = stamp; }
    void addApplyInfo(ApplyInfo& info){ m_vecApplyInfo.push_back(info); }
    bool delApplyInfo(int unionID);

private:
    bool        m_bHasUnion;        // �Ƿ��й���
    // �й��������
    bool        m_bHasAudit;        // �Ƿ��������Ϣ
    int			m_nUnionID;		    // ����id
    int         m_nTodayLiveness;    // ���ջ�Ծ��
    int         m_nTotalContribution;  // �ۼƹ���
    char        m_cPos;             // ְλ
    char		m_cUnionName[20];   // ������
    char        m_cUnionNotice[128];// ���ṫ��
    // û�й��������
    int         m_nApplyCount;       // ������Ĵ���
    int         m_nApplyStamp;       // ������ʱ���
    std::vector<ApplyInfo> m_vecApplyInfo;  //����Ĺ�����Ϣ

};

/////////////////////////////////////////////////////////////////////////�����ģ��
class CActivityInstanceModel
{
public:
	 bool init(void *data);
	 std::map<int, InstanceInfo>& getActivityInstance(){ return m_mapInstance; }

private:
	 std::map<int, InstanceInfo> m_mapInstance;
};

/////////////////////////////////////////////////////////////////////////�ʼ�ģ��
enum EMailType
{
	MAIL_TYPE_NORMAL,           // ��ͨ�ʼ�(�����������)
    MAIL_TYPE_UNIONTIPS,        // ���������ʼ�
    MAIL_TYPE_WEB,              // GM�ʼ�(�ֶ���д���⡢���ݵ�)
};

// �����ʼ�TIPS
enum EMailTipsType
{
    MAIL_TIPS_NON,                     // 
    MAIL_TIPS_NOPASS,                  // �������벻ͨ��
    MAIL_TIPS_KICK,                    // �����߳�
    MAIL_TIPS_APPOINT,                 // ����
    MAIL_TIPS_RELIEVE,                 // ��ְ
    MAIL_TIPS_PASS,                    // ��������ͨ��
};

#pragma pack(1)
struct DropItemInfo
{
    int id;					//��Ʒid
    int num;				//��Ʒ����
    int crit;				//��������
};

struct NoramlMailInfo
{
    int nMailID;						// �ʼ�ID
    int mailConfID;                     // �ʼ�����ID
    int sendTimeStamp;                  // ����ʱ���
    char szTitle[32];					// �ʼ�����
};

struct MailTips
{
    int tipsType;                        // ��ʾ����EMailTipsType
    int extend;                          // ��չ�ֶ�
	char unionName[32];                  // ��������
};
#pragma pack()

struct MailInfo
{
    bool isGetContent;                  // �Ƿ��ȡ����
    int mailID;                         // �ʼ�ID
    int mailType;					    // �ʼ����� EMailType
    int mailConfID;                     // ��Ϸ����ID
    int sendTimeStamp;                  // ����ʱ���
    std::string title;                  // ����
    std::string sender;					// ����
    std::string content;			    // ����
    std::vector<DropItemInfo>	items;	// ����
};

class CMailModel
{
public:
    CMailModel();
    ~CMailModel();

	 bool init(void *data);
	 bool addMail(const MailInfo& info);
     bool setMail(const MailInfo& info);
	 bool removeMail(int mailKey);
	 const MailInfo* getMail(int mailKey);
	 const std::map<int, MailInfo>& getMails(){ return m_mapMailInfo; }

	 void addUnionMail(const MailTips tips){ m_unionMailTip = tips; }
     void delUnionMail(){ memset(&m_unionMailTip, 0, sizeof(MailTips)); }
	 const MailTips& getUnionMail(){ return m_unionMailTip; }

     int getMailCount(){ return m_mapMailInfo.size(); }
private:
	 // �����ͨ�ʼ���ʱ��, map��key���ʼ� -100 - ID, ��ֹ��web�ʼ��ص�
	 int getMailKey(int mailID, int mailType);
private:
	 std::map<int, MailInfo> m_mapMailInfo;
	 // �����ʼ���ʾ, ��typeΪMAIL_TIPS_NONʱû����ʾ
	 MailTips				 m_unionMailTip;		
};

/////////////////////////////////////////////////////////////////////////�������ģ��
struct GoldTestInfo
{
	 int count;		//��ս����
	 int stamp;		//ʱ���
	 int damage;	//���˺�
	 int state;		//����״̬
};

class CGoldTestModel
{
public:
	 bool init(void* data);
	 int getCount(){ return m_sInfo.count; }
	 int getStamp(){ return m_sInfo.stamp; }
	 int getDamage(){ return m_sInfo.damage; }
	 int getState(int i);

	 void addCount(int count){ m_sInfo.count += count; }
	 void setStamp(int stamp){ m_sInfo.stamp = stamp; }
	 void addDamage(int damage){ m_sInfo.damage += damage; }
	 void setState(int i);  //����Ϊ��ȡ����
	 void setFlag(int flag){ m_sInfo.state = flag; }
    void resetGoldTest(int stamp);
private:
	 GoldTestInfo m_sInfo;
};

/////////////////////////////////////////////////////////////////////////Ӣ������ģ��
struct HeroTestInfo
{
	 int stamp;
	 int count;
};

class CHeroTestModel
{
public:
	 bool init(void* data);
	 int getCount(int i);
	 void setCount(int i, int count);
    void addCount(int i, int count);
	 int getStamp(){ return m_nStamp; }
	 void setStamp(int stamp ){ m_nStamp = stamp; }
    void resetHeroTest(int stamp);
private:
	 int m_nStamp;
	 std::map<int, int> m_mapCount;
};

/////////////////////////////////////////////////////////////////////////��������ģ��
struct TowerTestInfo
{
	 int stamp;		//ʱ���
	 int floor;     //��ǰ¥��
	 int event;     //��ǰ�¼�
	 int param;     //��ǰ�¼�����
	 int score;		//����
	 int count;		//Buff����
	 int crystal;	//ˮ��
	 int star;		//����
};

class CTowerTestModel
{
public:
	 bool init(void* data);
	 int getFloor(){ return m_nFloor; }
	 void setFloor(int f){ m_nFloor = f; }
	 void addBuff(int id);
private:
	 int m_nFloor;		//��ǰ¥��
};

/////////////////////////////////////////////////////////////////////////�����������ģ��
struct PersonalTaskInfo
{
	 int id;		// ����ID
	 int stage;		// ����ǰִ�е��Ĺؿ�
	 int status;	// ����״̬
	 int enemyLv;   // ���˵ȼ�
};

class CPersonalTaskModel
{
public:
	 CPersonalTaskModel();
	 int getResetTime(){ return m_nResetTime; }
	 void setResetTime(int time) { m_nResetTime = time; }
	 bool addTask(PersonalTaskInfo info);
	 std::map<int, PersonalTaskInfo> getTasks(){ return m_mapTaskInfo; }
	 bool setTask(PersonalTaskInfo info);
	 void clearPersonaTasks(){ m_mapTaskInfo.clear(); m_nResetTime = 0; }
private:
	 // ����ʱ��
	 int m_nResetTime;
	 // ��������
	 std::map<int, PersonalTaskInfo> m_mapTaskInfo;
};

/////////////////////////////////////////////////////////////////////////�����Ŷ�����ģ��
struct TeamTaskInfo 
{
	 int curTaskID;			// ��ǰ����ID
	 int endTime;           // ����ʱ���
	 int stage;             // �ڼ����ؿ�
	 int bossHp;            // ʣ��Ѫ��
	 int rewardBox;			// �����ڵ���ȡ��� 
	 int challengeCDTime;	// ��ս��ȴʱ��
	 int challengeTimes;	// ��ս����
	 int nextTargetTime;    // �¸�����Ŀ��ˢ��ʱ��
};

struct TeamHurtInfo
{
	 int userID;                 // ���ID
	 char userName[16];          // �������
	 int job;					 // ְλ
	 int headID;                 // ͷ��
	 int hurt;                   // �˺�ֵ
};

struct NextTeamTaskInfo
{
	 int taskID;                 // ����ID
	 int status;                 // ״̬ 1Ϊ�������´�����Ŀ��
};

class CTeamTaskModel
{
public:
	 CTeamTaskModel();
	 TeamTaskInfo getTeamTask(){ return m_teamTaskInfo; }
	 void setTeamTask(TeamTaskInfo info){ m_teamTaskInfo = info; }
	 std::vector<int> getNextTeamTasks(){ return m_vecNextTask; }
	 void setNextTeamTask(std::vector<int> nextTask){ m_vecNextTask = nextTask; }
	 void setNextTeamTaskID(int taskID){ m_nNextTaskID = taskID; }
	 int getNextTeamTaskID(){ return m_nNextTaskID; }
	 std::map<int, TeamHurtInfo> getHurtsInfo() { return m_mapHurtsInfo; }
	 bool setHurtInfo(TeamHurtInfo info);
	 bool addHurtInfo(TeamHurtInfo info);
	 void clearTask();
private:
	 // �´�Ŀ������ID
	 int m_nNextTaskID;
	 // ��ǰ������Ϣ
	 TeamTaskInfo m_teamTaskInfo;
	 // �´�����ID
	 std::vector<int> m_vecNextTask;
	 // BOSS�˺���Ϣ
	 std::map<int, TeamHurtInfo>	m_mapHurtsInfo;
};

/////////////////////////////////////////////////////////////////////////PVPģ��
enum MatchType
{
    MATCH_FAIRPVP,			//��ƽ����
    MATCH_CPN,				//������
};

enum EPvpRoomType
{
    PVPROOMTYPE_NONE,
    PVPROOMTYPE_PVP,				//pvp����		
    PVPROOMTYPE_ROBOT,				//�����˷���
    PVPROOMTYPE_CHAMPIONSHIP,		//����������
};

struct PvpInfo // pvp��Ϣ
{
	int BattleId;		   // battleId
    int ResetStamp;        // ˢ��ʱ���
    int DayBattleCount;    // ����ս����
    int DayContinusWin;    // ����ʤ����
    int DayWin;            // ��ʤ������
    int HistoryRank;       // ��ʷ�������
    int HistoryScore;      // ��ʷ��߻���
    int RewardFlag;        // ����������ȡ״̬
    int Rank;              // ��ǰ��λ
    int Score;             // ��ǰ����
    int ContinusWinTimes;  // ����ʤ��
    int DayMaxContinusWinTimes; // �����ʤ��
    int HistoryContinusWinTimes; // ��ʷ�����ʤ

    int CpnRank;					 //����������
    int CpnWeekResetStamp;           //������������ʱ��
    int CpnGradingNum;               //����������������
    int CpnGradingDval;              //�������������ֲ�
    int CpnIntegral;                 //��������������
    int CpnContinusWinTimes;         //��������λ����ʤ��
    int CpnHistoryHigestRank;        //��������ʷ�������
    int CpnHistoryHigestIntegral;    //��������ʷ��߻���
    int CpnHistoryContinusWinTimes;  //��������ʷ�����ʤ����

    int LastChestTime;               //���һ������ʱ��
    int ChestStatus;                 //����״̬ 1����ȡ
    int DayBuyChestTimes;            //�չ������
    int ChestInsurance;              //���״���

    PvpInfo operator=(const LoginPvpModelInfo &info)
    {
        BattleId = info.battleId;
        ResetStamp = info.dayResetStamp;
        Score = info.integral;
        Rank = info.rank;
        ContinusWinTimes = info.continusWinTimes;
        DayWin = info.dayWinTimes;
        DayContinusWin = info.dayContinusWinTimes;
        DayMaxContinusWinTimes = info.dayMaxContinusWinTimes;
        DayBattleCount = info.dayBattleTimes;
        HistoryRank = info.historyHighestRank;
        HistoryScore = info.historyHighestIntegral;
        RewardFlag = info.rewardFlag;
        HistoryContinusWinTimes = info.historyContinusWinTimes;

//         CpnRank = info.cpnRank;
//         CpnWeekResetStamp = info.cpnWeekResetStamp;
//         CpnGradingNum = info.cpnGradingNum;
//         CpnGradingDval = info.cpnGradingDval;
//         CpnIntegral = info.cpnIntegral;
//         CpnContinusWinTimes = info.cpnContinusWinTimes;
//         CpnHistoryHigestRank = info.cpnHistoryHigestRank;
//         CpnHistoryHigestIntegral = info.cpnHistoryHigestIntegral;
//         CpnHistoryContinusWinTimes = info.cpnHistoryContinusWinTimes;

        LastChestTime = info.lastChestGenStamp;
        ChestStatus = info.chestStatus;
        DayBuyChestTimes = info.dayBuyChestTimes;
        ChestInsurance = info.chestInsurance;

        return *this;
    }
};

class CPvpModel
{
public:
    CPvpModel();
    ~CPvpModel();

public:
    // ��ʼ��
    bool init(void* data);
    // ����״̬
    void resetPvp();
    // ��ȡPvp��Ϣ
    PvpInfo& getPvpInfo() { return m_PvpInfo; }
	// �Ƿ�����
	bool isReconnect();
	// �����Ƿ�����
	void setReconnect(bool isReconn);
	// ����battleid
	void setBattleId(int battleId);
    // ��������
    void setRank(int type, int rank);
	// ���pvp����
	int getRank() { return m_PvpInfo.Rank; }
    // ���û���
    void setScore(int type, int score);
    // ��ȡ����
    int getScore(int type);
    // ������ʷ�������
    void setHistoryRank(int type, int rank);
	// �����ʷ�������
    int getHistoryRank(int type);
    // ������ʷ��߻���
    void setHistoryScore(int type, int score);
	// �����ʷ��߻���
    int getHistoryScore(int type);
    // ��ȡ������״̬
    int getPvpTaskStatus(int type);
    // ����������״̬1
    void setPvpTaskStatus(int type);
	// ����ʤ��ʧ�ܼ�����ʤ��������
	void setDayTask(int result);
	// ���� 0=��ս������ 1=���ۼ�ʤ���� 2=����ʤ����
	void resetPvpTaskWithType(int taskType);
    // ����pvp����ʤ��
    void setContinueWinTimes(bool win);
    // ��ȡpvp��ʷ�����ʤ
    int getHistoryContinueWinTimes() { return m_PvpInfo.HistoryContinusWinTimes; }
    // ��������������
    void resetChampionArena();
    // ���÷�������
    void setRoomType(int type){ m_nRoomType = type; };
    // ��ȡ��������
    int getRoomType(){ return m_nRoomType; };
    // ����������+1
    void addGradingNum(){ m_PvpInfo.CpnGradingNum += 1; }
    // ���������ʱ��
    void setLastChestTime(int time){ m_PvpInfo.LastChestTime = time; }
    // ���ñ���״̬
    void setChestStatus(int status){ m_PvpInfo.ChestStatus = status; }
    // �����չ��������
    void setDayBuyChestTimes(int times){ m_PvpInfo.DayBuyChestTimes = times; }
    // ��ȡ�����ʱ��
    int getLastChestTime(){ return m_PvpInfo.LastChestTime; }

private:

	bool m_bIsReconnect;
    int m_nRoomType;
    PvpInfo m_PvpInfo;
};


/////////////////////////////////////////////////////////////////////////�̵�ģ��
// ��Ʒ�ṹ
#pragma pack(1)
struct ShopGoodsData
{
    int nIndex;					// ��Ʒ����
    int nGoodsShopID;			// ��ƷID
    int nGoodsID;				// ����ID
    int nGoodsNum;				// ���߸���
    int nCoinType;				// ��������
    int nCoinNum;				// �۸�
    int nSale;					// �ۿ�ֵ
};
#pragma pack()

//�̵�ṹ
struct ShopData
{
    int	nShopType;				//�̵�����
    int nCount;					//�̵�����Ʒ��
    int nCurCount;				//��ǰ��Ʒ��
    int nFreshedCount;		    //�Ѿ�ˢ�µĴ���
    int nNextFreshTime;			//�´�ˢ��ʱ���
    std::vector<ShopGoodsData> m_vecGoodsData;  //��Ʒ�ṹ�б�
};

// �̵�ģ��
class CShopModel
{
public:
    CShopModel();
    ~CShopModel();

public:
    /**
     * @brief  ��ʼ��
     * @param  data    �̵�����
     */
    bool init(void* data);

    /**
     * @brief  ��ȡ�̵����
     * @return �̵����
     */
    inline int getShopCount(){ return m_nShopCount; }

    /**
     * @brief  ��ȡ�̵�ģ������
     * @param  shopType    �̵�����
     * @return �����̵�ģ������
     */
    ShopData* getShopModelData(int shopType);

    /**
     * @brief  �����̵�ģ������
     * @param  shopType    �̵�����
     */
    void setShopModelData(const ShopData& data);

    /**
     * @brief   �ж��Ƿ�Ϊ�״γ�ֵ
     * @param   pID     ��Ʒ����
     */
    bool isFirstCharge(int pID);

    /**
    * @brief   �����״γ�ֵ״̬
    * @param   pID     ��Ʒ����
    */
    void setFirstChargeState(int pID);

private:
    int m_nShopCount;                       // �̵����
    std::map<int, ShopData> m_mapShopInfo;  // �̵��б�
    std::map<int, int>      m_mapDiamondShopData;
};


/////////////////////////////////////////////////////////////////////////��Ӫ�ģ��
// �̵�
struct SOperateActiveShop
{
    int nShopNum;				//�������
    std::vector<SLoginActiveShopData> m_vecActiveShopData;
};

// ����
struct SOperateActiveTask
{
    int nActiveTaskNum;				//�������
    std::vector<SLoginActiveTaskData> m_vecActiveTaskData;
};

//��Ӫ�ģ��
class COperateActiveModel
{
public:
    COperateActiveModel();
    ~COperateActiveModel();

    enum ActiveType
    {
        TYPE_NONE,
        TYPE_SHOP,
        TYPE_DROP,
        TYPE_TASK,
    };

    /**
     * @brief  ��ʼ��
     * @param  data    ��Ӫ�����
     */
    bool init(void* data);

    /**
    * @brief  ��ȡ�����
    * @return �����
    */
    inline int getActiveCount(){ return m_nActiveCount; }

    inline void delActiveCount(){ --m_nActiveCount; }

    /**
     * @brief   ��ȡ�������Ϣ
     */
    std::map<int, SLoginActiveData> getActiveData(){ return m_mapActiveData; }

    /**
     * @brief   ��ȡ�̵�����
     * @param   activeID    �ID
     * @return  �̵�����
     */
    SOperateActiveShop* getActiveShopData(int activeID);

    /**
     * @brief    �����Ѿ��������
     * @param    activeID   �ID
     * @param    giftID     ���ID
     * @param    buyTimes   �Ѿ��������
     */
    void setActiveShopBuyTimes(int activeID, int giftID, int buyTimes);

    /**
     * @brief   ��ȡ��������
     * @param   activeID    �ID
     * @return  ��������
     */
    SOperateActiveTask* getActiveTaskData(int activeID);

    /**
    * @brief    ����������ɽ���
    * @param    activeID   �ID
    * @param    taskID     ����ID
    * @param    value      ������ɽ���
    */
    void setActiveTaskProgress(int activeID, int taskID, int value);

    /**
    * @brief    ��������״̬
    * @param    activeID   �ID
    * @param    taskID     ����ID
    * @param    value      ������ɽ���
    */
    void setActiveTaskFinishFlag(int activeID, int taskID, int flag);

    /*
     * @brief   �Ƴ��Ѿ������Ļ
     */
    void removeActiveData(int activeID, int activeType);

private:
    int m_nActiveCount;
    std::map<int, SLoginActiveData> m_mapActiveData;   // ��Ӫ�������Ϣ, �ԻidΪ����
    std::map<int, SOperateActiveShop> m_mapActiveShop;   // �̵�����
    std::map<int, SOperateActiveTask> m_mapActiveTask;   // �̵�����
};

// ͷ������
class CHeadModel
{
public:
    CHeadModel();
    ~CHeadModel();

    /**
    * @brief  ��ʼ��
    * @param  data   �ѽ���ͷ���б�
    */
    bool init(void* data);

    /*
     * @brief   ��ȡ�ѽ���ͷ�� 
     * @return  �ѽ���ͷ���б�
     */
    //std::vector<int> getHeadList(){ return m_vecHeadID; }
    std::vector<int> getUnlockedHeads(){ return m_vecUnlockedHead; }

    /*
     * @brief   �ж�ͷ���Ƿ��ѽ���
     * @param   ͷ��ID
     * @return  �Ƿ��ѽ���
     */
    bool isUnlocked(int headID);

    /*
     * @brief   ���ͷ��
     * @param   ͷ��ID
     * @return  �ɹ�����true, ���򷵻�false
     */
    bool addHead(int headID);

private:
    int m_nNum;
    std::vector<int> m_vecUnlockedHead;
};

#endif