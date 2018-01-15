--[[
/*******************************************************************
** ������:	¬��
** ��  ��:  2016-10-12 10:31
** ��  ��:	1.0
** ��  ��:  ����Զ�������ͼ
** Ӧ  ��:
********************************************************************/
--]]
local scheduler = require("framework.scheduler")

local expeditionModel = getGameModel():getExpeditionModel()

local UIExpeditionHelpTips = class("UIExpeditionHelpTips", function()
    return require("common.UIView").new()
end)

function UIExpeditionHelpTips:ctor()

end

-- �����汻����ʱ�ص�
-- ֻ��ʼ��һ��
function UIExpeditionHelpTips:init(...)
    -- ����
    self.rootPath = ResConfig.UIExpeditionHelpTips.Csb2.main
    self.root = getResManager():getCsbNode(self.rootPath)
    self:addChild(self.root)

    local mainPanel = CsbTools.getChildFromPath(self.root, "MainPanel")
    mainPanel:setTouchEnabled(true)
    CsbTools.initButton(mainPanel, handler(self, self.onClick))

    local layout = ccui.Layout:create()
    layout:setBackGroundColorType(1);
    layout:setBackGroundColor(cc.c3b(8, 12, 18))
    layout:setBackGroundColorOpacity(225)
    layout:setName("preventTouch")
    layout:setContentSize(display.width, display.height)
    layout:setTouchEnabled(false)
    self.root:addChild(layout, -1)
    ccui.Helper:doLayout(layout)
end

-- �����汻��ʱ�ص�
-- ÿ�ε���Openʱ�ص�
function UIExpeditionHelpTips:onOpen(openerUIID, stage)
    local descText = CsbTools.getChildFromPath(self.root, "MainPanel/TalkText")
    descText:setString(CommonHelper.getStageString(stage.headDesc or 0))
    local nameText = CsbTools.getChildFromPath(self.root, "MainPanel/TitleText")
    nameText:setString(CommonHelper.getStageString(stage.headName or 0))
    local headNode = CsbTools.getChildFromPath(self.root, "MainPanel/Head")
    local head = cc.CSLoader:createNode(stage.headRes)
    if head then
        local action = cc.CSLoader:createTimeline(stage.headRes)
        head:runAction(action)
        action:play(stage.headTag, false)
        headNode:addChild(head)
    else
        print("Head Res Load Error", stage.headRes)
    end
end

-- ÿ�ν���Open�����������ʱ�ص�
function UIExpeditionHelpTips:onOpenAniOver()
end

-- �����汻�ر�ʱ�ص�
-- ÿ�ε���Closeʱ�ص�
-- ���Է��ض������
function UIExpeditionHelpTips:onClose()

end

-- �����汻�ö�ʱ�ص�
-- Openʱ������ص��ú���
function UIExpeditionHelpTips:onTop(preUIID, ...)

end

-- ��ǰ�������ص�
function UIExpeditionHelpTips:onClick(obj)
    local btnName = obj:getName()
    if btnName == "MainPanel" then             -- ����
        UIManager.close()
    end
end

return UIExpeditionHelpTips