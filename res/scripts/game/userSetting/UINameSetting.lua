--[[
/*******************************************************************
** ������:	¬��
** ��  ��:  2016-08-09 10:03
** ��  ��:	1.0
** ��  ��:  �����޸Ľ���
** Ӧ  ��:
********************************************************************/
--]]
local userModel = getGameModel():getUserModel()

local FloatText = {UnDevelop = 11, RepeatName = 51, NoEnoughDiamond = 5, OpenUnion = 301, UnNil = 432, UnLegal = 8}
local ChangeNameDiamond = 10 -- 10��ʯ

local UINameSetting = class("UINameSetting", function()
    return require("common.UIView").new()
end)

-- ���캯��
function UINameSetting:ctor()

end

-- �����汻����ʱ�ص�
-- ֻ��ʼ��һ��
function UINameSetting:init(...)
    self.rootPath = ResConfig.UINameSetting.Csb2.main
	self.root = getResManager():getCsbNode(self.rootPath)
	self:addChild(self.root)

    -- UI�ı�
    CsbTools.getChildFromPath(self.root, "MainPanel/TipsText"):setString(CommonHelper.getUIString(1669))
    CsbTools.getChildFromPath(self.root, "MainPanel/TipsText_2"):setString(string.format(CommonHelper.getUIString(1670), ChangeNameDiamond))
    -- ��ť�ı�
    CsbTools.getChildFromPath(self.root, "MainPanel/ConfrimButton/Node/PriceNum"):setString(ChangeNameDiamond)

    -- �رհ�ť
    local touchPanel = CsbTools.getChildFromPath(self.root, "MainPanel")
    CsbTools.initButton(touchPanel, handler(self, self.onClick))

    -- ȷ�ϰ�ť
    local btnConfirm = CsbTools.getChildFromPath(self.root, "MainPanel/ConfrimButton")
    CsbTools.initButton(btnConfirm, handler(self, self.onClick),nil,nil,"Node")

    -- �ı�����
    self.inputField = CsbTools.getChildFromPath(self.root, "MainPanel/InputField")
    self.inputField:addEventListener(handler(self, self.onInput))
end

-- �����汻��ʱ�ص�
-- ÿ�ε���Openʱ�ص�
function UINameSetting:onOpen(openerUIID, callback)
    self.callback = callback
    self.inputField:setString(userModel:getUserName())
end

-- ÿ�ν���Open�����������ʱ�ص�
function UINameSetting:onOpenAniOver()
end

-- �����汻�ر�ʱ�ص�
-- ÿ�ε���Closeʱ�ص�
-- ���Է��ض������
function UINameSetting:onClose()
    self.inputField:didNotSelectSelf()
    if self.renameHandler then
        local cmd = NetHelper.makeCommand(MainProtocol.User, UserProtocol.RenameSC)
        NetHelper.removeResponeHandler(cmd, self.renameHandler)
        self.renameHandler = nil
    end
end

-- �����汻�ö�ʱ�ص�
-- Openʱ������ص��ú���
function UINameSetting:onTop(preUIID, ...)

end

function UINameSetting:onClick(obj)
    local name = obj:getName()
    if "MainPanel" == name then
        UIManager.close()
    elseif "ConfrimButton" == name then
        obj.soundId = nil
        if not self:sendRenameCmd() then
            obj.soundId = MusicManager.commonSound.fail
        end
    end
end

function UINameSetting:onInput(obj, event)
    if 2 == event then
        -- ���д�����
        local newStr = FilterSensitive.FilterStr(obj:getString())
        -- ��������(6������12���ַ�)
        newStr = CommonHelper.limitStrLen(newStr, 6)
        obj:setString(newStr)
    end
end

-- ���͸�������
function UINameSetting:sendRenameCmd()
	-- ע���������
	local cmd = NetHelper.makeCommand(MainProtocol.User, UserProtocol.RenameSC)
    self.renameHandler = handler(self, self.acceptRenameCmd)
	NetHelper.setResponeHandler(cmd, self.renameHandler)

    -- �����������
    local changeName = self.inputField:getString()
    if changeName == userModel:getUserName() then
        CsbTools.addTipsToRunningScene(CommonHelper.getUIString(FloatText.RepeatName))
        return
    elseif 0 ~= userModel:getChangeNameFree() and userModel:getDiamond() < ChangeNameDiamond then -- ��ʯ����
        CsbTools.addTipsToRunningScene(CommonHelper.getUIString(FloatText.NoEnoughDiamond))
        return
    elseif string.find(changeName, " ") or string.len(changeName) <= 0 then
        CsbTools.addTipsToRunningScene(CommonHelper.getUIString(FloatText.UnNil))
        return
    else
        -- ���͸�����Ϣ
        local BufferData = NetHelper.createBufferData(MainProtocol.User, UserProtocol.RenameCS)
        BufferData:writeString(changeName)
        NetHelper.request(BufferData)

        self.inputField:didNotSelectSelf()
        return true
    end
end

-- ���ո�������
function UINameSetting:acceptRenameCmd(mainCmd, subCmd, buffData)
    -- ע����������
    local cmd = NetHelper.makeCommand(mainCmd, subCmd)
    NetHelper.removeResponeHandler(cmd, self.renameHandler)
    self.renameHandler = nil

    local result = buffData:readInt()
    if 1 == result then
        if 0 == userModel:getChangeNameFree() then -- 0���
            userModel:setChangeNameFree()
        else
            ModelHelper.addCurrency(UIAwardHelper.ResourceID.Diamond, -ChangeNameDiamond)
        end
        userModel:setUserName(self.inputField:getString())

        if self.callback and "function" == type(self.callback) then
            self.callback()
        end
    else
        print("Error: UINameSetting, change name fail!!!")
    end
    UIManager.close()
end

return UINameSetting