--[[
/*******************************************************************
** ������:	¬��
** ��  ��:  2016-09-14 18:52
** ��  ��:	1.0
** ��  ��:  ���ָ�������
** Ӧ  ��:
********************************************************************/
--]]

local userModel = getGameModel():getUserModel()

local PropTips = require("game.comm.PropTips")

local UINewBieBoon = class("UINewBieBoon", function()
    return require("common.UIView").new()
end)

function UINewBieBoon:ctor()
    self.rootPath = ResConfig.UINewBieBoon.Csb2.main
    self.root = getResManager():getCsbNode(self.rootPath)
    self:addChild(self.root)

    getChild(self.root, "MainPanel/InfoPanel/BarImage1/TitleText"):setString(CommonHelper.getUIString(1719))

    local backBtn = getChild(self.root, "BackButton")   -- �رհ�ť
    CsbTools.initButton(backBtn, handler(self, self.onClick))
end

-- �����汻����ʱ�ص�
-- ֻ��ʼ��һ��
function UINewBieBoon:init(...)
	--self.root
	--self.rootPath
end

-- �����汻��ʱ�ص�
-- ÿ�ε���Openʱ�ص�
function UINewBieBoon:onOpen(openerUIID, ...)
    self.propTips = PropTips.new()

    self.firstPayData = GetFirstPayData()
    if not self.firstPayData then
        return
    end

    -- ����״̬(0��δ��ȡ, 1������ȡ)
    self.firstPayFlag = userModel:getFirstPayFlag()
    self:setMoneyPanel()
    self:initFirstCharge()
    self:initGrowthFundPanel()
end

-- ÿ�ν���Open�����������ʱ�ص�
function UINewBieBoon:onOpenAniOver()
end

-- �����汻�ر�ʱ�ص�
-- ÿ�ε���Closeʱ�ص�
-- ���Է��ض������
function UINewBieBoon:onClose()
    self.propTips:removePropAllTips()
    self.propTips = nil
end

-- �����汻�ö�ʱ�ص�
-- Openʱ������ص��ú���
function UINewBieBoon:onTop(preUIID, ...)
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
    self:initGrowthFundPanel()
end

-- ��ǰ���水ť����ص�
function UINewBieBoon:onClick(obj)
    local btnName = obj:getName()
    if "BackButton" == btnName then
        UIManager.close()
    end
end

-- ���û������
function UINewBieBoon:setMoneyPanel()
    local goldCountLabel    = getChild(self.root, "GoldInfo/GoldPanel/GoldCountLabel")
    local gemCountLabel     = getChild(self.root, "GemInfo/GemPanel/GemCountLabel")
    goldCountLabel:setString(tostring(userModel:getGold()))
    gemCountLabel:setString(tostring(userModel:getDiamond()))
end

-- ��ʼ���׳����
function UINewBieBoon:initFirstCharge()
    local goodsID = self.firstPayData.GoodsID
    local goodsNum = self.firstPayData.GoodsNum
    -- ��ʼ���׳佱��
    for i = 1, 4 do
        local id = goodsID[i] or 0
        local num = goodsNum[i] or 0
        local propConf = getPropConfItem(id)
        if propConf then
            -- ����ͼƬ
            local allItem = getChild(self.root, "MainPanel/InfoPanel/FirstRecharge/AwardPanel/AllItem_" .. i)
            UIAwardHelper.setAllItemOfConf(allItem, propConf, num)
            -- ����tips
            local touchPanel = getChild(allItem, "MainPanel")
            self.propTips:addPropTips(touchPanel, propConf)
        else
            getChild(self.root, "MainPanel/InfoPanel/FirstRecharge/AwardPanel/AllItem_" .. i):setVisible(false)
        end
    end

    local node = getChild(self.root, "MainPanel/InfoPanel/FirstRecharge/ButtonState")

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
function UINewBieBoon:sendFirstChargeCmd()
    local buffData = NetHelper.createBufferData(MainProtocol.User, UserProtocol.FirstChargeCS)
    NetHelper.requestWithTimeOut(buffData,
        NetHelper.makeCommand(MainProtocol.User, UserProtocol.FirstChargeSC),
        handler(self, self.acceptFirstChargeCmd))
end

-- ������ȡ�׳佱������
function UINewBieBoon:acceptFirstChargeCmd(mainCmd, subCmd, buffData)
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



-- ���û������
function UINewBieBoon:initGrowthFundPanel()
    local gemNum = getChild(self.root, "MainPanel/InfoPanel/Buy1000Gem/GemNum")
    gemNum:setString(self.firstPayData.GrowGiftPrice)

    local node = getChild(self.root, "MainPanel/InfoPanel/Buy1000Gem/ButtonBuy")

    local startTime = userModel:getFundStartFlag()  -- ��ʼʱ��
    if startTime > 0 then
        local nowTime = getGameModel():getNow()
        local nowYear = tonumber(os.date("%Y", nowTime))
        local nowMon = tonumber(os.date("%m", nowTime))
        local nowDay = tonumber(os.date("%d", nowTime))
        if nowTime < startTime + self.firstPayData.GetTimes * 86400 and
            (getYear < nowYear or getMon < nowMon or getDay < nowDay) then

            CommonHelper.playCsbAnimation(node, "Green", false, nil)
        else
            CommonHelper.playCsbAnimation(node, "Grey", false, nil)
            -- �ոչ���
            if nowTime < startTime then
                getChild(node, "Button_Grey/Text"):setString(CommonHelper.getUIString(1723))
            -- ���һ������ȡ
            elseif nowTime >= startTime + (self.firstPayData.GetTimes - 1)* 86400 and 
                    getYear <= nowYear and getMon <= nowMon and getDay <= nowDay then

                getChild(node, "Button_Grey/Text"):setString(CommonHelper.getUIString(1725))
            else
                getChild(node, "Button_Grey/Text"):setString(CommonHelper.getUIString(503))
            end
        end
    else
        CommonHelper.playCsbAnimation(node, "Orange", false, nil)
    end
    
    local function onClick(obj)
        local btnName = obj:getName()
        -- ����
        if "Button_Orange" == btnName then
            -- ��ʯ
            local userDiamond = userModel:getDiamond()
            if userDiamond < self.firstPayData.GrowGiftPrice then
                CsbTools.addTipsToRunningScene(CommonHelper.getUIString(530))
            else
                self:sendGrowthFundCmd()
            end
        -- �콱
        elseif "Button_Green" == btnName then
            self:sendGrowthFundCmd()
        end
    end

    local buyBtn = getChild(node, "Button_Orange")
    CsbTools.initButton(buyBtn, onClick)
    local buyText = getChild(buyBtn, "Text")
    buyText:setString(CommonHelper.getUIString(173))
    local getBtn = getChild(node, "Button_Green")
    CsbTools.initButton(getBtn, onClick)
    local getText = getChild(getBtn, "Text")
    getText:setString(CommonHelper.getUIString(329))
end

-- ���͹���ɳ�������/��ȡ����
function UINewBieBoon:sendGrowthFundCmd()
    local buffData = NetHelper.createBufferData(MainProtocol.User, UserProtocol.GrowthFundCS)
    NetHelper.requestWithTimeOut(buffData,
        NetHelper.makeCommand(MainProtocol.User, UserProtocol.GrowthFundSC),
        handler(self, self.acceptGrowthFundCmd))
end

-- ���ճɳ�������/��ȡ����
function UINewBieBoon:acceptGrowthFundCmd(mainCmd, subCmd, buffData)
    local startTime = userModel:getFundStartFlag()    -- ��ʼʱ��

    local newTime = buffData:readInt()    -- ��ʼʱ��
    local getTime = buffData:readInt()      -- ��ȡʱ��
    userModel:setFundStartFlag(newTime)

    if startTime > 0 then
        ModelHelper.addCurrency(UIAwardHelper.ResourceID.Diamond, self.firstPayData.GiftDiamonds)
        self:setMoneyPanel()
        self:initGrowthFundPanel()
        -- ��ʾ����
        local awardData = {}
        local dropInfo = {}
        dropInfo.id = 2
        dropInfo.num = self.firstPayData.GiftDiamonds
        UIAwardHelper.formatAwardData(awardData, "dropInfo", dropInfo)
        if awardData and #awardData > 0 then
            UIManager.open(UIManager.UI.UIAward, awardData)
        end

        RedPointHelper.addCount(RedPointHelper.System.Boon, -1)
    else
        ModelHelper.addCurrency(UIAwardHelper.ResourceID.Diamond, -self.firstPayData.GrowGiftPrice)
        self:setMoneyPanel()
        self:initGrowthFundPanel()
        -- ��ʾ����ɹ�
        CsbTools.addTipsToRunningScene(CommonHelper.getUIString(1722))
        -- ��Ӷ�ʱ��������ȡ���
        ModelHelper.addBoonTime()
    end
end


return UINewBieBoon