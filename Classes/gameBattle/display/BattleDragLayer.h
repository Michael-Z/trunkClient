#ifndef __DRAGLAYER_H__
#define __DRAGLAYER_H__

#include "cocos2d.h"
#include "GameComm.h"
#include "UISkillRange.h"
#include "UISkillScreen.h"

typedef std::function<void()> CameraCallback;

//�����϶���
class CStageSetLayer : public cocos2d::Layer
{
public:
    CStageSetLayer();
    ~CStageSetLayer();

public:
    //��ʼ������, battleSizeΪս����������Ļ����, stageSizeΪ�϶�����Ļ����.
	bool initWithFile(const std::string &csbFile, float battleScreen, float stageScreen);
    //�϶��ƶ�
    void dragMove(float x);

private:

    float              m_fBattleScreen;           //ս���������
    float              m_fStageScreen;            //�������
};

//��ק��, ͨ������¼�������ק
class CBattleDragLayer : public cocos2d::Layer
{
public:
    CBattleDragLayer();
    ~CBattleDragLayer();

public:
    virtual bool init(int stageId, cocos2d::Node *parent, cocos2d::Node *battle);
    virtual void update(float delta);
	virtual void onEnter();
    virtual void onExit();

    // ��ʼ����һ������ͷЧ��
    void startCamera(int carmeraId);
    // ִ������ͷЧ����startCamera������ͷ�������Զ�ִ����һ������ͷ����ø÷���
    void doCamera(int carmeraId);
    // ֹͣ����ͷЧ��
    void stopCarmera();
    // �ƶ�����
    void dragMove(float x);
    // ��������ͷ�����ص�
    void setCameraFinishCallback(const CameraCallback& callback) { m_CameraFinishCallback = callback; }
    // ������ԭ
    void resetLayer();

	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
	void onTouchesCancelled(const std::vector<cocos2d::Touch*>&touches, cocos2d::Event *event);

    // ���ܴ���
    void onSkillReleaseEvent(void *data);
    // ����ȡ��
    void onSkillCancelEvent(void* data);

    // �������ܷ�Χ��
    void createSkillRange(float posX);

    // �Ƴ����ܷ�Χ��
    void removeSkillRange();

    // ����ɾ������
    void FadeRemoveSkillRange();

    // ������������
    void createSkillScreen();

    // �Ƴ���������
    void removeSkillScreen();

	// �Ŵ�
	void testScaleZoomIn(cocos2d::Ref *pSender);

	// ��С
	void testScaleZoomOut(cocos2d::Ref *pSender);

private:
    // �ͷż���
    void releaseSkill(int skillIndex, int pox);
	//void doScreenScale(float prevD, float curD, Vec2 centerPos);
	// �������Ű�ť
	void initTestZoomBtn();

    // ���ų���
    void scaleScene(float scale);

private:
	bool                    m_bMoveDisable;	            // �������϶�
    bool                    m_bIsMove;                  // �Ƿ���ק
    bool                    m_bIsScaling;               // �Ƿ�����
    int                     m_nSkillIndex;              // ��Ҫ�ͷŵļ����±�
    int                     m_nNextCarmeraId;           // ��һ��ִ�еľ�ͷID
    float                   m_fCarmeraTime;             // �������ʣ��ʱ��
    float                   m_fMoveTime;                // ��ͷ�ƶ�ʣ��ʱ��
    float                   m_fScaleTime;               // ��ͷ����ʣ��ʱ��
    float                   m_fScalePerSceond;          // ��ͷÿ���ƶ�����
    float                   m_fMovePerSceond;           // ��ͷÿ������ֵ
    float                   m_fCurCameraPosX;           // ��ǰ��ͷӦ���ƶ�����λ��
    float                   m_fBattleScreen;            // ս������������(1�����ΪStand��width), ��Ϊ��׼
    float                   m_fMinPosX;                 // ս������С��λ��
	float				    m_fRealScreenWidth;			// ��Ļ�����
	float				    m_fMaxScreenHeight;			// ��Ļ���߶�
    float                   m_fMinScale;				// ��С����ֵ>= 0.5
	float				    m_fOffsetY;					// �Ӿ�����ʱ��λ��
	Node*                   m_pParent;                  // ���ڵ�
	Node*                   m_pBattleLayer;             // ս���߼���
    CStageSetLayer*         m_pProspectLayer;           // Զ����
    CStageSetLayer*         m_pBackgroundLayer;         // ������
    CStageSetLayer*         m_pBattleBgLayer;           // ս����, ��ս���߼���ͬ��
    CStageSetLayer*         m_pForegroundLayer;         // ǰ����
    Vec2                    m_ScreenCenter;             // ��Ļ��������
    Vec2                    m_ParentOriginPos;          // ԭ�ȵ�λ��
	Size                    m_StandViewSize;            // ��׼��Ļ��С(960*640)
	cocos2d::Vec2		    m_CenterPos;				// �������ĵ�
    CameraCallback          m_CameraFinishCallback;     // ��ͷ���Ž�������õĻص�

private:
    CUISkillScreen*  m_SkillScreen;
    CUISkillRange*  m_SkillRange;
    float           m_fTouchPosX;
};

#endif
