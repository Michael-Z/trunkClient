--[[
����ʧ�ܽ���
]]

local UISettleAccountLose = class("UISettleAccountLose", function()
	return require("common.UIView").new()
end)

function UISettleAccountLose:ctor()
	--�������
	local okText = CommonHelper.getUIString(500)
	--ʧ�ܽ���ڵ�
	self.loseNode = getResManager():getCsbNode(ResConfig.UISettleAccountLose.Csb2.fail)
	self.loseNodeAct = cc.CSLoader:createTimeline(ResConfig.UISettleAccountLose.Csb2.fail)
	self.loseNode:runAction(self.loseNodeAct)
	self.tipTex = CsbTools.getChildFromPath(self.loseNode, "MainPanel/TipTex")
	self.loseOkBtn = CsbTools.getChildFromPath(self.loseNode, "MainPanel/ConfirmButtom")
	self.loseOkBtnSub = CsbTools.getChildFromPath(self.loseNode, "MainPanel/ConfirmButtom/ConfirmButtom")
	self.loseOkBtnAct = cc.CSLoader:createTimeline(ResConfig.UISettleAccountLose.Csb2.okBtn)
	self.loseOkBtnSub:runAction(self.loseOkBtnAct)
	CsbTools.getChildFromPath(self.loseOkBtnSub, "NameLabel"):setString(okText)

    self.loseOkBtn:addTouchEventListener(handler(self, self.loseOkButtonDown))
    self:addChild(self.loseNode)

    self:setGlobalZOrder(5)
	self:setLocalZOrder(5)
    CommonHelper.layoutNode(self.loseNode)
end

function UISettleAccountLose:onOpen()
	self.loseNode:setVisible(true)
	self.loseNodeAct:play("Open", false)
end

function UISettleAccountLose:onClose()

end

--ʧ�ܽ���"ȷ��"��ť�ص�
function UISettleAccountLose:loseOkButtonDown(obj, touchType)
	if touchType == 0 then --��ʼ���
		self.loseOkBtnAct:play("OnAnimation", false)
	elseif touchType == 2 then --�������
		self.loseOkBtnAct:play("Normal", false)
		--��ת�������ͼ���ߴ���
		self:backToScene()
	elseif touchType == 3 then --ȡ�����
		self.loseOkBtnAct:play("Normal", false)
	end
end

function UISettleAccountLose:backToScene()	
	-- ս����������Ե���������棬������뿪��Ϸ��ע������C++ս���㣬�Ա����ڴ�й¶
    -- �ͷŷ�����Դ
    finishBattle()
	-- ���ش�������
	if SceneManager.PrevScene then
		SceneManager.loadScene(SceneManager.PrevScene)
	else
		SceneManager.loadScene(SceneManager.Scene.SceneHall)
	end
end

return UISettleAccountLose
