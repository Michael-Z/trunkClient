--[[
/*******************************************************************
** ������:	¬��
** ��  ��:  2016-11-08 1:10
** ��  ��:	1.0
** ��  ��:  �׳佱��
** Ӧ  ��:
********************************************************************/
--]]

local userModel = getGameModel():getUserModel()

local PropTips = require("game.comm.PropTips")

local UIFirstRecharge = class("UIFirstRecharge", function()
    return require("common.UIView").new()
end)

function UIFirstRecharge:ctor()
    self.rootPath = ResConfig.UIFirstRecharge.Csb2.main
    self.root = getResManager():getCsbNode(self.rootPath)
    self:addChild(self.root)

    local closeBtn = getChild(self.root, "MainPanel/CloseButton")   -- �رհ�ť
    CsbTools.initButton(closeBtn, handler(self, self.onClick))
    local backBtn = getChild(self.root, "BackButton")   -- �رհ�ť
    CsbTools.initButton(backBtn, handler(self, self.onClick))
end

-- �����汻����ʱ�ص�
-- ֻ��ʼ��һ��
function UIFirstRecharge:init(...)
	--self.root
	--self.rootPath
end

-- �����汻��ʱ�ص�
-- ÿ�ε���Openʱ�ص�
function UIFirstRecharge:onOpen(openerUIID, ...)
    self.propTips = PropTips.new()

    self.firstPayData = GetFirstPayData()
    if not self.firstPayData then
        return
    end

    -- ����״̬(0��δ��ȡ, 1������ȡ)
    self.firstPayFlag = userModel:getFirstPayFlag()
    self:setMoneyPanel()
    self:initFirstCharge()
end

-- ÿ�ν���Open�����������ʱ�ص�
function UIFirstRecharge:onOpenAniOver()
end

-- �����汻�ر�ʱ�ص�
-- ÿ�ε���Closeʱ�ص�
-- ���Է��ض������
function UIFirstRecharge:onClose()
    self.propTips:removePropAllTips()
    self.propTips = nil
end

-- �����汻�ö�ʱ�ص�
-- Openʱ������ص��ú���
function UIFirstRecharge:onTop(preUIID, ...)
    local flag = userModel:getFirstPayFlag()

    -- �׳�״̬�б�
    if self.firstPayFlag ~= flag then
        self.firstPayFlag = flag
        --
        if 1 == flag then
            RedPointHelper.addBoonRed()
        else
            RedPointHelper.addCount(RedPointHelper.System.Boon, -1)
        end
    end

    self:setMoneyPanel()
    self:initFirstCharge()
end

-- ��ǰ���水ť����ص�
function UIFirstRecharge:onClick(obj)
    local btnName = obj:getName()
    if "BackButton" == btnName then
        UIManager.close()
    elseif "CloseButton" == btnName then
        UIManager.close()
    end
end

-- ���û������
function UIFirstRecharge:setMoneyPanel()
    local goldCountLabel    = getChild(self.root, "GoldInfo/GoldPanel/GoldCountLabel")
    local gemCountLabel     = getChild(self.root, "GemInfo/GemPanel/GemCountLabel")
    goldCountLabel:setString(tostring(userModel:getGold()))
    gemCountLabel:setString(tostring(userModel:getDiamond()))
end

-- ��ʼ���׳����
function UIFirstRecharge:initFirstCharge()
    local goodsID = self.firstPayData.GoodsID
    local goodsNum = self.firstPayData.GoodsNum
    -- ��ʼ���׳佱��
    for i = 1, 4 do
        local id = goodsID[i] or 0
        local num = goodsNum[i] or 0
        local propConf = getPropConfItem(id)
        if propConf then
            -- ����ͼƬ
            local allItem = getChild(self.root, "MainPanel/AllItem_" .. i)
            UIAwardHelper.setAllItemOfConf(allItem, propConf, num)
            -- ����tips
            local touchPanel = getChild(allItem, "MainPanel")
            self.propTips:addPropTips(touchPanel, propConf)
        else
            getChild(self.root, "MainPanel/AllItem_" .. i):setVisible(false)
        end
    end

    local node = getChild(self.root, "MainPanel/ButtonState")
    local vipPayment = getGameModel():getUserModel():getPayment()
    if vipPayment and vipPayment > 0 then
        if 0 == self.firstPayFlag then
            CommonHelper.playCsbAnimation(node, "Green", false, nil)
        else
            CommonHelper.playCsbAnimation(node, "Grey", false, nil)
        end
    else
        CommonHelper.playCsbAnimation(node, "Orange", false, nil)
    end

    local function onClick(obj)
        local btnName = obj:getName()
        -- ��ֵ
        if "Button_Orange" == btnName then
            UIManager.open(UIManager.UI.UIShop, ShopType.DiamondShop)
        -- �콱
        elseif "Button_Green" == btnName then
            self:sendFirstChargeCmd()
        end
    end
    local chargeBtn = getChild(node, "Button_Orange")
    CsbTools.initButton(chargeBtn, onClick)
    local chargeText = getChild(chargeBtn, "Text")
    chargeText:setString(CommonHelper.getUIString(23))
    local getBtn = getChild(node, "Button_Green")
    CsbTools.initButton(getBtn, onClick)
    local getText = getChild(getBtn, "Text")
    getText:setString(CommonHelper.getUIString(329))
    getChild(node, "Button_Grey/Text"):setString(CommonHelper.getUIString(503))
end

-- ������ȡ�׳佱������
function UIFirstRecharge:sendFirstChargeCmd()
    local buffData = NetHelper.createBufferData(MainProtocol.User, UserProtocol.FirstChargeCS)
    NetHelper.requestWithTimeOut(buffData,
        NetHelper.makeCommand(MainProtocol.User, UserProtocol.FirstChargeSC),
        handler(self, self.acceptFirstChargeCmd))
end

-- ������ȡ�׳佱������
function UIFirstRecharge:acceptFirstChargeCmd(mainCmd, subCmd, buffData)
    local flag = buffData:readInt()
    userModel:setFirstPayFlag(flag)
    self.firstPayFlag = flag
    self:initFirstCharge()

    -- ��ʾ����
    local goodsID = self.firstPayData.GoodsID
    local goodsNum = self.firstPayData.GoodsNum
    local awardData = {}
    local dropInfo = {}
    for i = 1, 4 do
        local id = goodsID[i]
        if not id or id <= 0 then
            break
        end
        local num = goodsNum[i]
        if not num or num <= 0 then
            break
        end
        dropInfo.id = id
        dropInfo.num = num
        UIAwardHelper.formatAwardData(awardData, "dropInfo", dropInfo)
    end
    if awardData and #awardData > 0 then
        UIManager.open(UIManager.UI.UIAward, awardData)
    end
end


return UIFirstRecharge